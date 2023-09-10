﻿#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"

void UIFlytekVoiceBPLibrary::StartASR_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle,
	const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	FIFlytekVoiceManage::Get()->StartASR_ByWebSocket(WorldContextObject, OutHandle, InConfigInfo, InASRSocketTextDelegate);
}

void UIFlytekVoiceBPLibrary::StopASR_ByWebSocket(int32 InHandle)
{
	FIFlytekVoiceManage::Get()->StopASR_ByWebSocket(InHandle);
}

void UIFlytekVoiceBPLibrary::StartASD_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle,
	const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate)
{
	FIFlytekVoiceManage::Get()->StartASD_ByWebSocket(WorldContextObject, OutHandle, InConfigInfo, InASDSocketTextDelegate);
}

void UIFlytekVoiceBPLibrary::StopASD_ByWebSocket(int32 InHandle)
{
	FIFlytekVoiceManage::Get()->StopASD_ByWebSocket(InHandle);
}

void UIFlytekVoiceBPLibrary::StartTTS_ByWebSocket(const UObject* WorldContextObject, const FString& content,
	const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay, bool bSaveToFile, const FString& filePath)
{
	FIFlytekVoiceManage::Get()->StartTTS_ByWebSocket(WorldContextObject, content, InConfigInfo, bAutoPlay, bSaveToFile, filePath);
}

void UIFlytekVoiceBPLibrary::StartTextModeration(const UObject* WorldContextObject, const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate)
{
	FIFlytekVoiceManage::Get()->StartTextModeration(WorldContextObject, content, InConfigInfo, InTMHttpDelegate);
}

void UIFlytekVoiceBPLibrary::ChatSparkDesk(const UObject* WorldContextObject, const FString& content,
	const FIFlytekSDInfo& InConfigInfo, FSDSocketDelegate InSDSocketDelegate)
{
	FIFlytekVoiceManage::Get()->ChatSparkDesk(WorldContextObject, content, InConfigInfo, InSDSocketDelegate);
}
