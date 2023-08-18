#include "Core/IFlytekSocketSubsystem.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceLog.h"

#include "WebSocketsModule.h"

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
