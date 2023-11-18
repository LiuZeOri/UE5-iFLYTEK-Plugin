#include "IFlytekVoiceBPLibrary.h"
#include "IFlytekVoiceManage.h"
#include "Sound/SoundHandle.h"

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

void UIFlytekVoiceBPLibrary::StartTTS_ByWebSocket_WithCompletedDelegate(const UObject* WorldContextObject,
	const FString& content, const FIFlytekTTSInfo& InConfigInfo, const FString& filePath,
	FTTSSaveFileCompletedDelegate InTTSSaveFileCompletedDelegate, bool bAutoPlay)
{
	FIFlytekVoiceManage::Get()->StartTTS_ByWebSocket_WithCompletedDelegate(WorldContextObject, content, InConfigInfo, filePath, InTTSSaveFileCompletedDelegate, bAutoPlay);
}

void UIFlytekVoiceBPLibrary::ChatSparkDesk(const UObject* WorldContextObject, const FString& content,
                                           const FIFlytekSDInfo& InConfigInfo, FSDSocketDelegate InSDSocketDelegate)
{
	FIFlytekVoiceManage::Get()->ChatSparkDesk(WorldContextObject, content, InConfigInfo, InSDSocketDelegate);
}

void UIFlytekVoiceBPLibrary::UnloadHttpMoudle()
{
	FModuleManager::Get().UnloadModule("HTTP");
	UE_LOG(LogTemp, Log, TEXT("HTTP module unload."));
}

void UIFlytekVoiceBPLibrary::LoadHttpMoudle()
{
	FModuleManager::Get().LoadModuleChecked("HTTP");
}

void UIFlytekVoiceBPLibrary::PlaySoundByFile(const FString& InFilePath)
{
	SoundHandle::PlaySoundByFile(InFilePath);
}
