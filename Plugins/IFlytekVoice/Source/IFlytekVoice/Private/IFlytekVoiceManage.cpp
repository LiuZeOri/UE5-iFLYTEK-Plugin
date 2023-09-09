#include "IFlytekVoiceManage.h"

#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "Kismet/GameplayStatics.h"

#include "Subsystem/IFlytekASRSocketSubsystem.h"
#include "Subsystem/IFlytekASDSocketSubsystem.h"
#include "Subsystem/IFlytekTTSSocketSubsystem.h"
#include "Subsystem/IFlytekTMHttpSubsystem.h"

FIFlytekVoiceManage* FIFlytekVoiceManage::Manage = nullptr;

FIFlytekVoiceManage::FIFlytekVoiceManage()
{
}

FIFlytekVoiceManage::~FIFlytekVoiceManage()
{
}

FIFlytekVoiceManage* FIFlytekVoiceManage::Get()
{
	if (!Manage)
	{
		Manage = new FIFlytekVoiceManage();
		
		// 初始化日志
		Manage->InitLog();
	}
	return Manage;
}

void FIFlytekVoiceManage::Destroy()
{
	if (Manage)
	{
		delete Manage;
	}
	Manage = NULL;
}

void FIFlytekVoiceManage::StartASR_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	if (!WorldContextObject)
	{
		return;
	}
	
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekASRSocketSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UIFlytekASRSocketSubsystem>();
	
	IFlytekASRSocketSubsystem->CreateSocket(InConfigInfo, InASRSocketTextDelegate);
	
	IFlytekASRSocketSubsystem->SendAudioData(OutHandle);
}

void FIFlytekVoiceManage::StopASR_ByWebSocket(int32 InHandle)
{
	IFLYTEK_LOG_PRINT(TEXT("Stop ASR."));
	
	IFlytekASRSocketSubsystem->StopSendAudioData(InHandle);
	
	IFlytekASRSocketSubsystem->CloseSocket();
}

void FIFlytekVoiceManage::StartASD_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle,
	const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate)
{
	if (!WorldContextObject)
	{
		return;
	}
	
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekASDSocketSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UIFlytekASDSocketSubsystem>();

	IFlytekASDSocketSubsystem->CreateSocket(InConfigInfo, InASDSocketTextDelegate);
	
	IFlytekASDSocketSubsystem->SendAudioData(OutHandle, InConfigInfo);
}

void FIFlytekVoiceManage::StopASD_ByWebSocket(int32 InHandle)
{
	IFLYTEK_LOG_PRINT(TEXT("Stop ASD."));
	
	IFlytekASDSocketSubsystem->StopSendAudioData(InHandle);
	
	IFlytekASDSocketSubsystem->CloseSocket();
}

void FIFlytekVoiceManage::StartTTS_ByWebSocket(const UObject* WorldContextObject, const FString& content, const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay, bool bSaveToFile, const FString& filePath)
{
	if (!WorldContextObject)
	{
		return;
	}
	
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekTTSSocketSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UIFlytekTTSSocketSubsystem>();

	IFlytekTTSSocketSubsystem->CreateSocket(InConfigInfo, bAutoPlay, bSaveToFile, filePath);
	
	IFlytekTTSSocketSubsystem->SendData(content, InConfigInfo);
}

void FIFlytekVoiceManage::StartTextModeration(const UObject* WorldContextObject, const FString& content, const FIFlytekTMInfo& InConfigInfo)
{
	if (!WorldContextObject)
	{
		return;
	}
	
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekTMHttpSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UIFlytekTMHttpSubsystem>();

	IFlytekTMHttpSubsystem->SendRequest(content, InConfigInfo);
}

void FIFlytekVoiceManage::InitLog()
{
	// 配置初始化
	FIFlytekVoiceConfig::Get()->Init();
}
