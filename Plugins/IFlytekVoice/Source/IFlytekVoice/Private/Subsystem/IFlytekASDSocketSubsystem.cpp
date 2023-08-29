#include "Subsystem/IFlytekASDSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "WebSocketsModule.h"
#include "JSON/IFlytekVoiceJson.h"
#include "Sound/RecordingCollection.h"
#include "Util/hmacsha256.h"

struct FASDAbandonable : FNonAbandonableTask
{
	FASDAbandonable(const FSimpleDelegate& InThreadDelegate)
		:ThreadDelegate(InThreadDelegate)
	{
		
	}

	~FASDAbandonable()
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

void UIFlytekASDSocketSubsystem::CreateSocket(const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate)
{
	// 加载模块
	FModuleManager::Get().LoadModuleChecked("WebSockets");
	
	// 握手阶段，设置参数

	// 接口鉴权

	// 1.设置signature
	// 获取当前时间戳，RFC1123格式
	FString date = FDateTime::Now().UtcNow().ToHttpDate();

	// 生成signature的原始字段
	FString signatureOrigin = FString::Printf(TEXT("host: %s\ndate: %s\nGET /v2/iat HTTP/1.1"), *InConfigInfo.host, *date);

	// 使用hmac-sha256算法结合apiSecret对signatureOrigin签名，获得签名后的摘要signatureSha
	// 使用base64编码对signatureSha进行编码获得最终的signature
	FString signature = HMACSHA256(FIFlytekVoiceConfig::Get()->UserInfo.apiSecret, signatureOrigin);

	// 2.根据以上信息拼接authorization base64编码前（authorizationOrigin）的字符串
	FString authorizationOrigin = FString::Printf(TEXT("api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""),
		*FIFlytekVoiceConfig::Get()->UserInfo.apiKeyTTS,
		*signature);

	// 最后再对authorization_origin进行base64编码获得最终的authorization参数
	FString authorization = FBase64::Encode(authorizationOrigin);

	// 3.构造请求
	// 将data进行urlencode
	date.ReplaceInline(TEXT(","), TEXT("%2C"));
	date.ReplaceInline(TEXT(" "), TEXT("%20"));
	date.ReplaceInline(TEXT(":"), TEXT("%3A"));

	// 拼接URL
	FString URL = FString::Printf(TEXT("%s?authorization=%s&date=%s&host=%s"),
		*InConfigInfo.serverURL,
		*authorization,
		*date,
		*InConfigInfo.host);
	
	// 建立Socket连接
	Socket = FWebSocketsModule::Get().CreateWebSocket(URL, InConfigInfo.serverProtocol);

	Socket->OnConnected().AddUObject(this, &UIFlytekASDSocketSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UIFlytekASDSocketSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekASDSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekASDSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UIFlytekASDSocketSubsystem::OnMessageSent);

	Socket->Connect();

	// 绑定委托
	ASDSocketTextDelegate = InASDSocketTextDelegate;
}

void UIFlytekASDSocketSubsystem::CloseSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}
}

void UIFlytekASDSocketSubsystem::SendAudioData(int32& OutHandle)
{
	OutHandle = GetASDHandle();

	(new FAutoDeleteAsyncTask<FASDAbandonable>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekASDSocketSubsystem::SendAudioData_Thread,
		OutHandle)))->StartBackgroundTask();
}

void UIFlytekASDSocketSubsystem::SendAudioData_Thread(int32 InHandle)
{
	if (IsASDHandleExist(InHandle))
	{
		// 开始发送实时音频流
		IFLYTEK_LOG_PRINT(TEXT("Start recording and sending audio data."))
		
		TSharedPtr<FRecordingCollection> RecordingCollection = FRecordingCollection::CreateRecordingCollection();

		FRecordingConfig InConfig;
		RecordingCollection->InitConfig(InConfig);

		RecordingCollection->StartSpeak();

		bool bBreak = FindASDHandle(InHandle);
		while (!bBreak)
		{
			bBreak = FindASDHandle(InHandle);
						
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

	RemoveASDHandle(InHandle);
}

void UIFlytekASDSocketSubsystem::StopSendAudioData(int32 InHandle)
{
	FScopeLock ScopeLock(&ASDMutex);
	if (bool* InValue = ASDPool.Find(InHandle))
	{
		*InValue = true;
	}

	// 上传完成后，客户端需发送一个特殊的binary message到服务端作为结束标识
	std::string end = "{\"end\": true}";
	std::vector<uint8> binaryData (end.begin(), end.end());
	Socket->Send(binaryData.data(), binaryData.size());
	ASDSocketTextDelegate.Clear();
}

void UIFlytekASDSocketSubsystem::OnConnected()
{
	IFLYTEK_WARNING_PRINT(TEXT("%s"), *FString(__FUNCTION__));
}

void UIFlytekASDSocketSubsystem::OnConnectionError(const FString& Error)
{
	IFLYTEK_ERROR_PRINT(TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);

	ASDSocketTextDelegate.Clear();
}

void UIFlytekASDSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
}

void UIFlytekASDSocketSubsystem::OnMessage(const FString& Message)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);

	// 解析Json数据
	FASRSocketResponded Responded;
	IFlytekVoiceJson::ASRSocketRespondedToString(Message, Responded);

	// 返回结果处理
	if (Responded.type == 0)
	{
		IFLYTEK_LOG_PRINT(TEXT("%s | %s"), *Responded.src, *Responded.dst);

		// 结果回调
		//ASDSocketTextDelegate.ExecuteIfBound(Responded, srcFinalString, dstFinalString);
	}
	else if (Responded.type == 1)
	{

		// 结果回调
		//ASDSocketTextDelegate.ExecuteIfBound(Responded, srcBuffString, dstBuffString);

	}
	else if (Responded.type == -1 && !Responded.action.Equals("result"))
	{
		// 说明异常，直接回调
		ASDSocketTextDelegate.ExecuteIfBound(Responded, TEXT(""), TEXT(""));
	}
}

void UIFlytekASDSocketSubsystem::OnMessageSent(const FString& MessageString)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}

int32 UIFlytekASDSocketSubsystem::GetASDHandle()
{
	FScopeLock ScopeLock(&ASDMutex);

	int32 InHandle = CreateASDUniqueHandle();
	ASDPool.Add(InHandle, false);

	return InHandle;
}

bool UIFlytekASDSocketSubsystem::RemoveASDHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASDMutex);

	return ASDPool.Remove(InHandle) != 0;
}

bool UIFlytekASDSocketSubsystem::FindASDHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&ASDMutex);
	if (bool* InValue = ASDPool.Find(InHandle))
	{
		return *InValue;
	}
	return true;
}

bool UIFlytekASDSocketSubsystem::IsASDHandleExist(int32 InHandle) const
{
	return ASDPool.Find(InHandle) != nullptr;
}

int32 UIFlytekASDSocketSubsystem::CreateASDUniqueHandle()
{
	int32 Index = FMath::RandRange(0, 999999999);

	if(ASDPool.Find(Index))
	{
		return CreateASDUniqueHandle();
	}
	return Index;
}
