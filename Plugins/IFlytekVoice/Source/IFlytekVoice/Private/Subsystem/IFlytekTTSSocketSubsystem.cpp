#include "Subsystem/IFlytekTTSSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "Util/hmacsha256.h"
#include "JSON/IFlytekVoiceJson.h"
#include "WebSocketsModule.h"


void UIFlytekTTSSocketSubsystem::CreateSocket(const FIFlytekTTSInfo& InConfigInfo)
{
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

	Socket->OnConnected().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UIFlytekTTSSocketSubsystem::OnMessageSent);

	Socket->Connect();
}

void UIFlytekTTSSocketSubsystem::CloseSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}
}

void UIFlytekTTSSocketSubsystem::SendData(const FIFlytekTTSInfo& InConfigInfo)
{
	// 上传数据
	FString sendJsonString;
	IFlytekVoiceJson::TTSSocketRequestToJson(InConfigInfo, sendJsonString);
	Socket->Send(sendJsonString);
}

void UIFlytekTTSSocketSubsystem::OnConnected()
{
	IFLYTEK_WARNING_PRINT(TEXT("%s"), *FString(__FUNCTION__));
}

void UIFlytekTTSSocketSubsystem::OnConnectionError(const FString& Error)
{
	IFLYTEK_ERROR_PRINT(TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);
}

void UIFlytekTTSSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
}

void UIFlytekTTSSocketSubsystem::OnMessage(const FString& Message)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
}

void UIFlytekTTSSocketSubsystem::OnMessageSent(const FString& MessageString)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}
