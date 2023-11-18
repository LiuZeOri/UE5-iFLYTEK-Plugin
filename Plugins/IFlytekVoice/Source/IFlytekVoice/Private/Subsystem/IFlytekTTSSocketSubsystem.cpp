#include "Subsystem/IFlytekTTSSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "Util/hmacsha256.h"
#include "JSON/IFlytekVoiceJson.h"
#include "WebSocketsModule.h"
#include "Sound/SoundHandle.h"
#include "Sound/SoundFormatHandle.h"


void UIFlytekTTSSocketSubsystem::CreateSocket(const FIFlytekTTSInfo& InConfigInfo, bool bInAutoPlay, bool bInSaveToFile, const FString& InFilePath)
{
	// 加载模块
	FModuleManager::Get().LoadModuleChecked("WebSockets");

	// 清空，释放内存
	PCMData.Empty();
	PCMData.Shrink();
	WAVData.Empty();
	WAVData.Shrink();
	
	bAutoPlay = bInAutoPlay;
	bSaveToFile = bInSaveToFile;
	filePath = InFilePath;
	if (InConfigInfo.GetAudioSampleRateString().Equals(TEXT("audio/L16;rate=8000")))
		sampleRate = 8000;
	else if (InConfigInfo.GetAudioSampleRateString().Equals(TEXT("audio/L16;rate=16000")))
		sampleRate = 16000;
	
	// 握手阶段，设置参数

	// 接口鉴权

	// 1.设置signature
	// 获取当前时间戳，RFC1123格式
	FString date = FDateTime::Now().UtcNow().ToHttpDate();

	// 生成signature的原始字段
	FString signatureOrigin = FString::Printf(TEXT("host: %s\ndate: %s\nGET /v2/tts HTTP/1.1"), *InConfigInfo.host, *date);

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

	Socket->OnConnected().AddUObject(this, &Super::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &Super::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &Super::OnMessageSent);

	Socket->Connect();
}

void UIFlytekTTSSocketSubsystem::SendData(const FString& content, const FIFlytekTTSInfo& InConfigInfo)
{
	// 上传数据
	FString sendJsonString;
	IFlytekVoiceJson::TTSSocketRequestToJson(InConfigInfo, GetAfterEncodeText(content), sendJsonString);
	Socket->Send(sendJsonString);
}

void UIFlytekTTSSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	Super::OnClosed(StatusCode, Reason, bWasClean);
}

void UIFlytekTTSSocketSubsystem::OnMessage(const FString& Message)
{
	Super::OnMessage(Message);

	// 解析Json数据
	FTTSSocketResponded Responded;
	IFlytekVoiceJson::TTSSocketRespondedToString(Message, Responded);

	// 合成成功
	if (Responded.code == 0)
	{
		PCMData.Append(Responded.GetAudioDecodeData());
	}

	// 合成完毕
	if (Responded.status == 2)
	{

		SoundFormatHandle::PCMToWAVByMemory(1, sampleRate, PCMData, WAVData);
		
		// 内存播放音频
		if (bAutoPlay)
		{
			const uint8* SoundPtr = WAVData.GetData();
			SoundHandle::PlaySoundByMemory(SoundPtr);
		}

		// 保存文件
		if (bSaveToFile)
		{
			FFileHelper::SaveArrayToFile(WAVData, *filePath);
		}

		// 保存完成事件回调
		if (bUseDelegate)
		{
			TTSSaveFileCompletedDelegate.ExecuteIfBound();
		}
		bUseDelegate = false;
		
		// 关闭子系统
		CloseSocket();
	}
}

void UIFlytekTTSSocketSubsystem::SetDelegate(FTTSSaveFileCompletedDelegate InTTSSaveFileCompletedDelegate)
{
	TTSSaveFileCompletedDelegate = InTTSSaveFileCompletedDelegate;
	bUseDelegate = true;
}

FString UIFlytekTTSSocketSubsystem::GetAfterEncodeText(const FString& InText)
{
	// 将FString转换为UTF-16编码的字节数组
	TArray<uint8> OutArray;
	const TCHAR* CharPtr = *InText;
	while (*CharPtr)
	{
		const TCHAR Char = *CharPtr++;
		// 转换字符到UTF-16编码并处理字节顺序（小端）
		OutArray.Add(static_cast<uint8>(Char & 0xFF));
		OutArray.Add(static_cast<uint8>((Char >> 8) & 0xFF));
	}

	return FBase64::Encode(OutArray.GetData(), OutArray.Num());
}
