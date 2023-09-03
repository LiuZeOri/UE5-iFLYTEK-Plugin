#include "Subsystem/IFlytekASRSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "WebSocketsModule.h"
#include "JSON/IFlytekVoiceJson.h"
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

void UIFlytekASRSocketSubsystem::CreateSocket(const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	// 加载模块
	FModuleManager::Get().LoadModuleChecked("WebSockets");
	
	// 握手阶段，设置参数
	
	// signa生成
	
	// 1.获取baseString，baseString由appid和当前时间戳ts拼接而成
	// 获取当前时间戳
	int32 ts = FDateTime::Now().UtcNow().ToUnixTimestamp();
	FString baseString = FString::Printf(TEXT("%s%i"), *FIFlytekVoiceConfig::Get()->UserInfo.appID, ts);

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
		*FIFlytekVoiceConfig::Get()->UserInfo.appID,
		ts,
		*signa);

	// 设置语种
	URL.Appendf(TEXT("&lang=%s"), *InConfigInfo.GetLanguageTypeString());
	
	// 设置翻译功能
	if (InConfigInfo.bTranslation)
	{
		URL.Appendf(TEXT("&transType=normal&transStrategy=%s&targetLang=%s"),
			*InConfigInfo.GetTranslationStrategyString(),
			*InConfigInfo.GetTranslationLanguageTypeString());
	}

	// 设置是否过滤标点
	if (InConfigInfo.bFilterPunctuation)
	{
		URL.Appendf(TEXT("&punc=0"));
	}

	// 设置是否启用垂直领域个性化参数
	if (InConfigInfo.bUsePersonalizationParameter)
	{
		URL.Appendf(TEXT("&pd=%s"), *InConfigInfo.GetPersonalizationParameterString());
	}

	// 设置是否启用远近场切换
	if (InConfigInfo.bUseNearOrFarField)
	{
		URL.Appendf(TEXT("&vadMdn=%s"), *InConfigInfo.GetFieldType());
	}

	// 设置是否开角色分离
	if (InConfigInfo.bUseRoleSeparation)
	{
		URL.Appendf(TEXT("&roleType=2"));
	}

	// 语言识别模式选择
	URL.Appendf(TEXT("&engLangType=%s"), *InConfigInfo.GetLanguageRecognitionMode());
	
	// 建立Socket连接
	Socket = FWebSocketsModule::Get().CreateWebSocket(URL, InConfigInfo.serverProtocol);

	Socket->OnConnected().AddUObject(this, &UIFlytekASRSocketSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UIFlytekASRSocketSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekASRSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekASRSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UIFlytekASRSocketSubsystem::OnMessageSent);

	Socket->Connect();

	// 绑定委托
	ASRSocketTextDelegate = InASRSocketTextDelegate;
}

void UIFlytekASRSocketSubsystem::CloseSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}
}

void UIFlytekASRSocketSubsystem::SendAudioData(int32& OutHandle)
{
	OutHandle = GetASRHandle();

	(new FAutoDeleteAsyncTask<FASRAbandonable>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekASRSocketSubsystem::SendAudioData_Thread,
		OutHandle)))->StartBackgroundTask();
}

void UIFlytekASRSocketSubsystem::SendAudioData_Thread(int32 InHandle)
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

		// 上传完成后，客户端需发送一个特殊的binary message到服务端作为结束标识
		std::string end = "{\"end\": true}";
		std::vector<uint8> binaryData (end.begin(), end.end());
		Socket->Send(binaryData.data(), binaryData.size());

		IFLYTEK_LOG_PRINT(TEXT("Send Audio done."));
	}

	RemoveASRHandle(InHandle);
}

void UIFlytekASRSocketSubsystem::StopSendAudioData(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);
	if (bool* InValue = ASRPool.Find(InHandle))
	{
		*InValue = true;
	}
	
	ASRSocketTextDelegate.Clear();
}

void UIFlytekASRSocketSubsystem::OnConnected()
{
	IFLYTEK_WARNING_PRINT(TEXT("%s"), *FString(__FUNCTION__));
}

void UIFlytekASRSocketSubsystem::OnConnectionError(const FString& Error)
{
	IFLYTEK_ERROR_PRINT(TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);

	ASRSocketTextDelegate.Clear();
}

void UIFlytekASRSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);

	// 清空，释放内存
	srcFinalString.Empty();
	srcFinalString.Shrink();
	dstFinalString.Empty();
	dstFinalString.Shrink();
}

void UIFlytekASRSocketSubsystem::OnMessage(const FString& Message)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);

	// 解析Json数据
	FASRSocketResponded Responded;
	IFlytekVoiceJson::ASRSocketRespondedToString(Message, Responded);

	// 错误处理
	if (Responded.action.Equals(TEXT("error")))
	{
		CloseSocket();
	}

	// 返回结果处理
	if (Responded.type == 0)
	{
		IFLYTEK_LOG_PRINT(TEXT("%s | %s"), *Responded.src, *Responded.dst);
		srcFinalString += Responded.src;
		dstFinalString += Responded.dst;
		// 结果回调
		ASRSocketTextDelegate.ExecuteIfBound(Responded, srcFinalString, dstFinalString);
	}
	else if (Responded.type == 1)
	{
		srcBuffString = srcFinalString + Responded.src;
		dstBuffString = dstFinalString + Responded.dst;
		// 结果回调
		ASRSocketTextDelegate.ExecuteIfBound(Responded, srcBuffString, dstBuffString);
		srcBuffString = TEXT("");
		dstBuffString = TEXT("");
	}
	else if (Responded.type == -1 && !Responded.action.Equals("result"))
	{
		// 说明异常，直接回调
		ASRSocketTextDelegate.ExecuteIfBound(Responded, TEXT(""), TEXT(""));
	}
}

void UIFlytekASRSocketSubsystem::OnMessageSent(const FString& MessageString)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}

int32 UIFlytekASRSocketSubsystem::GetASRHandle()
{
	FScopeLock ScopeLock(&ASRMutex);

	int32 InHandle = CreateASRUniqueHandle();
	ASRPool.Add(InHandle, false);

	return InHandle;
}

bool UIFlytekASRSocketSubsystem::RemoveASRHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);

	return ASRPool.Remove(InHandle) != 0;
}

bool UIFlytekASRSocketSubsystem::FindASRHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASRMutex);
	if (bool* InValue = ASRPool.Find(InHandle))
	{
		return *InValue;
	}
	return true;
}

bool UIFlytekASRSocketSubsystem::IsASRHandleExist(int32 InHandle) const
{
	return ASRPool.Find(InHandle) != nullptr;
}

int32 UIFlytekASRSocketSubsystem::CreateASRUniqueHandle()
{
	int32 Index = FMath::RandRange(0, 999999999);

	if(ASRPool.Find(Index))
	{
		return CreateASRUniqueHandle();
	}
	return Index;
}
