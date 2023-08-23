#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"

void UIFlytekVoiceBPLibrary::StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	FIFlytekVoiceManage::Get()->StartASR_ByWebSocket(OutHandle, InConfigInfo, InASRSocketTextDelegate);
}

void UIFlytekVoiceBPLibrary::StopASR_ByWebSocket(int32 InHandle)
{
	FIFlytekVoiceManage::Get()->StopASR_ByWebSocket(InHandle);
}

void UIFlytekVoiceBPLibrary::StartTTS_ByWebSocket(const FString& content, const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay, bool bSaveToFile, const FString& filePath)
{
	FIFlytekVoiceManage::Get()->StartTTS_ByWebSocket(content, InConfigInfo, bAutoPlay, bSaveToFile, filePath);
}
