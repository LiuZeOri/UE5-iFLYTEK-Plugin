#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

class UIFlytekASRSocketSubsystem;
class UIFlytekTTSSocketSubsystem;

/**
 * 功能实现类，为单例类
 */
class IFLYTEKVOICE_API FIFlytekVoiceManage
{
public:
	FIFlytekVoiceManage();
	~FIFlytekVoiceManage();
	
	static FIFlytekVoiceManage* Get();
	static void Destroy();

public:
	// WebSocket方法实现实时语音转写
	void StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate);
	void StopASR_ByWebSocket(int32 InHandle);

	// WebSocket方法实现语音合成
	void StartTTS_ByWebSocket(const FIFlytekTTSInfo& InConfigInfo);

protected:
	void InitLog();

private:
	static FIFlytekVoiceManage* Manage;

	UIFlytekASRSocketSubsystem* IFlytekASRSocketSubsystem = nullptr;
	UIFlytekTTSSocketSubsystem* IFlytekTTSSocketSubsystem = nullptr;
};
