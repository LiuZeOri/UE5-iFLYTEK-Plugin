#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "IWebSocket.h"
#include "Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "IFlytekSocketSubsystem.generated.h"

/**
 * 科大讯飞ASR实时语音转写Socket子系统
 * 官方Web API文档：https://www.xfyun.cn/doc/asr/rtasr/API.html#%E6%8E%A5%E5%8F%A3%E8%AF%B4%E6%98%8E
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSocket(const FIFlytekASRInfo& InConfigInfo);
	void CloseSocket();
	
	void SendAudioData(int32& OutHandle, FASRSocketTextDelegate InASRSocketTextDelegate);
	void SendAudioData_Thread(int32 InHandle);
	void StopSendAudioData(int32 InHandle);

protected:
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket = nullptr;
	FASRSocketTextDelegate ASRSocketTextDelegate;

private:
	int32 GetASRHandle();
	bool RemoveASRHandle(int32 InHandle);
	bool FindASRHandle(int32 InHandle);
	bool IsASRHandleExist(int32 InHandle) const;
	int32 CreateASRUniqueHandle();
	
private:
	// 线程池
	TMap<int32, bool> ASRPool;
	// 锁
	FCriticalSection ASRMutex;

};
