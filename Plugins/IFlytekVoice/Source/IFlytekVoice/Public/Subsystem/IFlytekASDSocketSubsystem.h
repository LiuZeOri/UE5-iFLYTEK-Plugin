#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "IWebSocket.h"
#include "Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "IFlytekASDSocketSubsystem.generated.h"

/**
 * 科大讯飞ASD实时语音听写（流式版）Socket子系统
 * 项目需求：实时语音识别，用于1分钟内的即时语音转文字技术，可动态修正
 * 官方Web API文档：https://www.xfyun.cn/doc/asr/voicedictation/API.html#%E6%8E%A5%E5%8F%A3%E8%AF%B4%E6%98%8E
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekASDSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void CreateSocket(const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate);
	void CloseSocket();
	
	void SendAudioData(int32& OutHandle, const FIFlytekASDInfo& InConfigInfo);
	void SendAudioData_Thread(int32 InHandle, const FIFlytekASDInfo InConfigInfo);
	void StopSendAudioData(int32 InHandle);

protected:
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket;
	FASDSocketTextDelegate ASDSocketTextDelegate;

private:
	int32 GetASDHandle();
	bool RemoveASDHandle(int32 InHandle);
	bool FindASDHandle(int32 InHandle);
	bool IsASDHandleExist(int32 InHandle) const;
	int32 CreateASDUniqueHandle();
	
private:
	// 线程池
	TMap<int32, bool> ASDPool;
	// 锁
	FCriticalSection ASDMutex;
};
