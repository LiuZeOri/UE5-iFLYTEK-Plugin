#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"

void UIFlytekVoiceBPLibrary::InitASRWebSocketSubsystem()
{
	FIFlytekVoiceManage::Get()->InitASRWebSocketSubsystem();
}

void UIFlytekVoiceBPLibrary::StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo)
{
	FIFlytekVoiceManage::Get()->StartASR_ByWebSocket(OutHandle, InConfigInfo);
}

void UIFlytekVoiceBPLibrary::StopASR_ByWebSocket(int32 InHandle)
{
	FIFlytekVoiceManage::Get()->StopASR_ByWebSocket(InHandle);
}
