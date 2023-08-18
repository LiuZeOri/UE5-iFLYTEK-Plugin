#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"

void UIFlytekVoiceBPLibrary::StartASR_ByWebSocket(const FIFlytekASRInfo& InConfigInfo)
{
	FIFlytekVoiceManage::Get()->StartASR_ByWebSocket(InConfigInfo);
}
