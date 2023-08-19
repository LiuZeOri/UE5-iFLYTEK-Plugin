#include "Core/IFlytekSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "WebSocketsModule.h"
#include "Sound/RecordingCollection.h"

struct FASRAbandonable : FNonAbandonableTask
{
	FASRAbandonable(const FSimpleDelegate& InThreadDelegate)
		:ThreadDelegate(InThreadDelegate)
	{
		
	}

	~FASRAbandonable()
	{

	}

	void DoWork()
	{
		ThreadDelegate.ExecuteIfBound();
	}

	// ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FASRAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	FSimpleDelegate ThreadDelegate;
};

void UIFlytekSocketSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UIFlytekSocketSubsystem::CreateSocket(const FIFlytekASRInfo& InConfigInfo)
{
	// 握手阶段，设置参数
	
	// signa生成
	
	// 1.获取baseString，baseString由appid和当前时间戳ts拼接而成
	// 获取当前时间戳
	int32 ts = FDateTime::Now().UtcNow().ToUnixTimestamp();
	FString baseString = FString::Printf(TEXT("%s%i"), *FIFlytekVoiceConfig::Get()->UserInfo.appId, ts);

	// 2.对baseString进行MD5
	baseString = FMD5::HashAnsiString(*baseString);

	// 3.以apiKey为key对MD5之后的baseString进行HmacSHA1加密，然后再对加密后的字符串进行base64编码
	FSHAHash outHash;
	FSHA1::HMACBuffer(TCHAR_TO_ANSI(*FIFlytekVoiceConfig::Get()->UserInfo.apiKeyASR),
		FIFlytekVoiceConfig::Get()->UserInfo.apiKeyASR.Len(),
		TCHAR_TO_ANSI(*baseString),
		baseString.Len(),
		outHash.Hash);
	FString signa = FBase64::Encode(outHash.Hash, 20);
	signa.ReplaceInline(TEXT("="), TEXT("%3D"));

	// 拼接请求URL
	FString URL = FString::Printf(TEXT("%s?appid=%s&ts=%i&signa=%s"),
		*InConfigInfo.serverURL,
		*FIFlytekVoiceConfig::Get()->UserInfo.appId,
		ts,
		*signa);

	// 设置语种
	URL.Appendf(TEXT("&lang=%s"), *InConfigInfo.GetLanguageString());
	
	// 设置其他...
	
	// 建立Socket连接
	Socket = FWebSocketsModule::Get().CreateWebSocket(URL, InConfigInfo.serverProtocol);

	Socket->OnConnected().AddUObject(this, &UIFlytekSocketSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UIFlytekSocketSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UIFlytekSocketSubsystem::OnMessageSent);

	Socket->Connect();
}

void UIFlytekSocketSubsystem::CloseSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}
}

void UIFlytekSocketSubsystem::SendAudioData(int32& OutHandle)
{
	OutHandle = GetASRHandle();
	
	(new FAutoDeleteAsyncTask<FASRAbandonable>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekSocketSubsystem::SendAudioData_Thread,
		OutHandle)))->StartBackgroundTask();
}

void UIFlytekSocketSubsystem::SendAudioData_Thread(int32 InHandle)
{
	if (IsASRHandleExist(InHandle))
	{
		// 开始发送实时音频流
		IFLYTEK_LOG_PRINT(TEXT("Start recording and sending audio data."))
		
		TSharedPtr<FRecordingCollection> RecordingCollection = FRecordingCollection::CreateRecordingCollection();

		FRecordingConfig InConfig;
		RecordingCollection->InitConfig(InConfig);

		RecordingCollection->StartSpeak();

		bool bBreak = FindASRHandle(InHandle);
		while (!bBreak)
		{
			bBreak = FindASRHandle(InHandle);
						
			int32 DataSize = 0;
			uint8* Ptr = RecordingCollection->GetBuffer(DataSize);
							
			int CurrentIndex = 0;
			while (CurrentIndex < DataSize)
			{
				uint8 TmpData[FRAME_SIZE] = { 0 };
	
				int32 Remaining = DataSize - CurrentIndex;
				if (Remaining > FRAME_SIZE)
				{
					Remaining = FRAME_SIZE;
				}
	
				FMemory::Memcpy(TmpData, Ptr, Remaining);
	
				Ptr += Remaining;
				CurrentIndex += Remaining;
	
				Socket->Send(TmpData, Remaining);
			}
		}

		RecordingCollection->StopSpeak();
	}

	IFLYTEK_LOG_PRINT(TEXT("Send Audio done."));

	RemoveASRHandle(InHandle);
}

void UIFlytekSocketSubsystem::StopSendAudioData(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);
	if (bool* InValue = ASRPool.Find(InHandle))
	{
		*InValue = true;
	}
}

void UIFlytekSocketSubsystem::OnConnected()
{
	IFLYTEK_WARNING_PRINT(TEXT("%s"), *FString(__FUNCTION__));
}

void UIFlytekSocketSubsystem::OnConnectionError(const FString& Error)
{
	IFLYTEK_ERROR_PRINT(TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);
}

void UIFlytekSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
}

void UIFlytekSocketSubsystem::OnMessage(const FString& Message)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
}

void UIFlytekSocketSubsystem::OnMessageSent(const FString& MessageString)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}

int32 UIFlytekSocketSubsystem::GetASRHandle()
{
	FScopeLock ScopeLock(&ASRMutex);

	int32 InHandle = CreateASRUniqueHandle();
	ASRPool.Add(InHandle, false);

	return InHandle;
}

bool UIFlytekSocketSubsystem::RemoveASRHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);

	return ASRPool.Remove(InHandle) != 0;
}

bool UIFlytekSocketSubsystem::FindASRHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);
	if (bool* InValue = ASRPool.Find(InHandle))
	{
		return *InValue;
	}
	return true;
}

bool UIFlytekSocketSubsystem::IsASRHandleExist(int32 InHandle) const
{
	return ASRPool.Find(InHandle) != nullptr;
}

int32 UIFlytekSocketSubsystem::CreateASRUniqueHandle()
{
	int32 Index = FMath::RandRange(0, 999999999);

	if(ASRPool.Find(Index))
	{
		return CreateASRUniqueHandle();
	}
	return Index;
}
