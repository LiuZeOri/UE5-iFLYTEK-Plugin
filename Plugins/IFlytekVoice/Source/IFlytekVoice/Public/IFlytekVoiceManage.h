#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

class UIFlytekSocketSubsystem;

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

protected:
	void InitLog();

private:
	static FIFlytekVoiceManage* Manage;

	UIFlytekSocketSubsystem* IFlytekSocketSubsystem = nullptr;
};
