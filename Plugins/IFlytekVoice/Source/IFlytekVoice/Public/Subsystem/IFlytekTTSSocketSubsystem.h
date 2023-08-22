#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "IWebSocket.h"
#include "Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "IFlytekTTSSocketSubsystem.generated.h"

/**
 * 科大讯飞TTS语音合成（流式版）Socket子系统
 * 官方Web API文档：https://www.xfyun.cn/doc/tts/online_tts/API.html#%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%B5%81%E7%A8%8B
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekTTSSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void CreateSocket(const FIFlytekTTSInfo& InConfigInfo);
	void CloseSocket();

protected:
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket;

};
