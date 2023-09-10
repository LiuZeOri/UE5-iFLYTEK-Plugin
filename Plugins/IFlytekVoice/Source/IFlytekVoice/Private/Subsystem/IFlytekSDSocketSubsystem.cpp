#include "Subsystem/IFlytekSDSocketSubsystem.h"
#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "WebSocketsModule.h"
#include "JSON/IFlytekVoiceJson.h"
#include "Util/hmacsha256.h"

void UIFlytekSDSocketSubsystem::CreateSocket(const FIFlytekSDInfo& InConfigInfo, FSDSocketDelegate InSDSocketDelegate)
{
	// 加载模块
	FModuleManager::Get().LoadModuleChecked("WebSockets");
	
	// 握手阶段，设置参数

	// 接口鉴权

	// 1.设置signature
	// 获取当前时间戳，RFC1123格式
	FString date = FDateTime::Now().UtcNow().ToHttpDate();

	// 生成tmp
	FString tmp = FString::Printf(TEXT("host: %s\ndate: %s\nGET %s HTTP/1.1"), *InConfigInfo.GetHost(), *date, *InConfigInfo.GetPath());

	// 使用hmac-sha256算法结合apiSecret对tmp签名，获得签名后的摘要tmp_sha
	// 使用base64编码对tmp_sha进行编码获得最终的signature
	FString signature = HMACSHA256(FIFlytekVoiceConfig::Get()->UserInfo.apiSecret, tmp);

	// 2.根据以上信息拼接authorization base64编码前（authorizationOrigin）的字符串
	FString authorizationOrigin = FString::Printf(TEXT("api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""),
		*FIFlytekVoiceConfig::Get()->UserInfo.apiKeySD,
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
		*InConfigInfo.GetServerURL(),
		*authorization,
		*date,
		*InConfigInfo.GetHost());

	// 建立Socket连接
	Socket = FWebSocketsModule::Get().CreateWebSocket(URL, TEXT("ws"));

	Socket->OnConnected().AddUObject(this, &Super::OnConnected);
	Socket->OnConnectionError().AddUObject(this, &Super::OnConnectionError);
	Socket->OnClosed().AddUObject(this, &UIFlytekSDSocketSubsystem::OnClosed);
	Socket->OnMessage().AddUObject(this, &UIFlytekSDSocketSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &Super::OnMessageSent);

	Socket->Connect();

	// 绑定委托
	SDSocketDelegate = InSDSocketDelegate;
}

void UIFlytekSDSocketSubsystem::SendData(const FString& content, const FIFlytekSDInfo& InConfigInfo)
{
	// 上传数据
	FString sendJsonString;
	IFlytekVoiceJson::SDSocketRequestToJson(InConfigInfo, content, sendJsonString);
	Socket->Send(sendJsonString);
}

void UIFlytekSDSocketSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	Super::OnClosed(StatusCode, Reason, bWasClean);

	SDSocketDelegate.Clear();
}

void UIFlytekSDSocketSubsystem::OnMessage(const FString& Message)
{
	Super::OnMessage(Message);

	IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
}
