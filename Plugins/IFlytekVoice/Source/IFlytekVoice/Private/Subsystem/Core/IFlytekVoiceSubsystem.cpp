#include "Subsystem/Core/IFlytekVoiceSubsystem.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"

void UIFlytekVoiceSubsystem::CloseSocket()
{
	if (Socket.IsValid() && Socket->IsConnected())
	{
		Socket->Close();
	}
}

void UIFlytekVoiceSubsystem::OnConnected()
{
	IFLYTEK_WARNING_PRINT(TEXT("%s"), *FString(__FUNCTION__));
}

void UIFlytekVoiceSubsystem::OnConnectionError(const FString& Error)
{
	IFLYTEK_ERROR_PRINT(TEXT("%s Error:%s"), *FString(__FUNCTION__), *Error);
}

void UIFlytekVoiceSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	IFLYTEK_WARNING_PRINT(TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__), StatusCode, *Reason, bWasClean);
}

void UIFlytekVoiceSubsystem::OnMessage(const FString& Message)
{
	//IFLYTEK_WARNING_PRINT(TEXT("%s Message:%s"), *FString(__FUNCTION__), *Message);
}

void UIFlytekVoiceSubsystem::OnMessageSent(const FString& MessageString)
{
	//IFLYTEK_WARNING_PRINT(TEXT("%s MessageString:%s"), *FString(__FUNCTION__), *MessageString);
}
