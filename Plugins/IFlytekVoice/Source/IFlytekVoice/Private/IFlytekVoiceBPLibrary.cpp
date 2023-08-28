#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"

void UIFlytekVoiceBPLibrary::StartASR_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	FIFlytekVoiceManage::Get()->StartASR_ByWebSocket(WorldContextObject, OutHandle, InConfigInfo, InASRSocketTextDelegate);
}

void UIFlytekVoiceBPLibrary::StopASR_ByWebSocket(int32 InHandle)
{
	FIFlytekVoiceManage::Get()->StopASR_ByWebSocket(InHandle);
}

void UIFlytekVoiceBPLibrary::StartTTS_ByWebSocket(const UObject* WorldContextObject, const FString& content, const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay, bool bSaveToFile, const FString& filePath)
{
	FIFlytekVoiceManage::Get()->StartTTS_ByWebSocket(WorldContextObject, content, InConfigInfo, bAutoPlay, bSaveToFile, filePath);
}
