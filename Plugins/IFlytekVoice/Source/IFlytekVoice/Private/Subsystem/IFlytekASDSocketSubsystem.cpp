#include "Subsystem/IFlytekASDSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "WebSocketsModule.h"
#include "JSON/IFlytekVoiceJson.h"
#include "Sound/RecordingCollection.h"
#include "Util/hmacsha256.h"

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
		*FIFlytekVoiceConfig::Get()->UserInfo.apiKeyASD,
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

	Socket->OnConnected().AddUObject(this, &Super::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &Super::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &Super::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekASDSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &Super::OnMessageSent);

	Socket->Connect();

	// 绑定委托
	ASDSocketTextDelegate = InASDSocketTextDelegate;
}

void UIFlytekASDSocketSubsystem::SendAudioData(int32& OutHandle, const FIFlytekASDInfo& InConfigInfo)
{
	OutHandle = GetASDHandle();

	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekASDSocketSubsystem::SendAudioData_Thread,
		OutHandle, InConfigInfo)))->StartBackgroundTask();
}

void UIFlytekASDSocketSubsystem::SendAudioData_Thread(int32 InHandle, const FIFlytekASDInfo InConfigInfo)
{
	// 首帧请求
	FString sendFirstFrameJsonString;
	IFlytekVoiceJson::ASDSocketFirstFrameRequestToJson(InConfigInfo, sendFirstFrameJsonString);
	Socket->Send(sendFirstFrameJsonString);

	// 开始录音
	if (IsASDHandleExist(InHandle))
	{
		// 开始发送实时音频流
		IFLYTEK_LOG_PRINT(TEXT("Start recording and sending audio data."))
		
		TSharedPtr<FRecordingCollection> RecordingCollection = FRecordingCollection::CreateRecordingCollection();

		// 设置录音采样率
		FRecordingConfig InRecordingConfig;
		if (InConfigInfo.format == EAudioFormat::rate8k)
		{
			InRecordingConfig.SampleRate = 8000;
		}
		
		RecordingCollection->InitConfig(InRecordingConfig);

		RecordingCollection->StartSpeak();

		// 发送音频数据
		FString sendJsonString;
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

				IFlytekVoiceJson::ASDSocketRequestToJson(InConfigInfo, FBase64::Encode(TmpData, Remaining), sendJsonString);
	
				Socket->Send(sendJsonString);
			}
		}

		RecordingCollection->StopSpeak();

		// 最后一帧请求
		FString sendLastFrameJsonString;
		IFlytekVoiceJson::ASDSocketLastFrameRequestToJson(InConfigInfo, sendLastFrameJsonString);
		Socket->Send(sendLastFrameJsonString);

		IFLYTEK_LOG_PRINT(TEXT("Send Audio done."));
	}
	
	RemoveASDHandle(InHandle);
}

void UIFlytekASDSocketSubsystem::StopSendAudioData(int32 InHandle)
{
	FScopeLock ScopeLock(&ASDMutex);
	if (bool* InValue = ASDPool.Find(InHandle))
	{
		*InValue = true;
	}

	decoder.TextDiscard();
	ASDSocketTextDelegate.Clear();
}

void UIFlytekASDSocketSubsystem::OnMessage(const FString& Message)
{
	Super::OnMessage(Message);

	// 解析Json数据
	FASDSocketResponded Responded;
	IFlytekVoiceJson::ASDSocketRespondedToString(Message, Responded);
	
	if (Responded.code == 0)
	{
		ASDText te = GetASDText(Responded);
		
		decoder.TextDecode(te);

		ASDSocketTextDelegate.ExecuteIfBound(false, decoder.OutTextString());
		
		if (Responded.data.status == 2)
		{
			// status = 2 说明数据全部返回完毕，可以关闭连接，释放资源
			IFLYTEK_LOG_PRINT(TEXT("ASD end"));

			ASDSocketTextDelegate.ExecuteIfBound(true, decoder.OutTextString());

			decoder.TextDiscard();
			
			CloseSocket();
		}
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("Responded error"));
	}
}

ASDText UIFlytekASDSocketSubsystem::GetASDText(const FASDSocketResponded& InResponded)
{
	ASDText text;
	FString outText;
	for (FASDSocketRespondedWs WsTmp : InResponded.data.result.ws)
	{
		outText.Append(WsTmp.cw[0]);
	}
	text.sn = InResponded.data.result.sn;
	text.text = outText;
	text.rg = InResponded.data.result.rg;
	text.pgs = InResponded.data.result.pgs;
	
	return text;
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
