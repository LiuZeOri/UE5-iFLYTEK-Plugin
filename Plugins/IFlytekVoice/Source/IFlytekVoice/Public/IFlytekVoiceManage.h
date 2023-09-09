#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

class UIFlytekASRSocketSubsystem;
class UIFlytekASDSocketSubsystem;
class UIFlytekTTSSocketSubsystem;
class UIFlytekTMHttpSubsystem;

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
	void StartASR_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate);
	void StopASR_ByWebSocket(int32 InHandle);

	// WebSocket方法实现实时语音听写
	void StartASD_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate);
	void StopASD_ByWebSocket(int32 InHandle);

	// WebSocket方法实现语音合成
	void StartTTS_ByWebSocket(const UObject* WorldContextObject, const FString& content, const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay = true, bool bSaveToFile = false, const FString& filePath = TEXT(""));

	// 文本检测TM
	void StartTextModeration(const UObject* WorldContextObject, const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate);

protected:
	void InitLog();

private:
	static FIFlytekVoiceManage* Manage;

	UIFlytekASRSocketSubsystem* IFlytekASRSocketSubsystem = nullptr;
	UIFlytekASDSocketSubsystem* IFlytekASDSocketSubsystem = nullptr;
	UIFlytekTTSSocketSubsystem* IFlytekTTSSocketSubsystem = nullptr;
	UIFlytekTMHttpSubsystem* IFlytekTMHttpSubsystem = nullptr;
};
