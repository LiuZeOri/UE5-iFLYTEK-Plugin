#include "IFlytekVoiceManage.h"

#include "AITestsCommon.h"
#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"

#include "Subsystem/IFlytekASRSocketSubsystem.h"
#include "Subsystem/IFlytekTTSSocketSubsystem.h"

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

void FIFlytekVoiceManage::StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate)
{
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekASRSocketSubsystem = FAITestHelpers::GetWorld()->GetGameInstance()->GetSubsystem<UIFlytekASRSocketSubsystem>();
	
	IFlytekASRSocketSubsystem->CreateSocket(InConfigInfo);
	
	IFlytekASRSocketSubsystem->SendAudioData(OutHandle, InASRSocketTextDelegate);
}

void FIFlytekVoiceManage::StopASR_ByWebSocket(int32 InHandle)
{
	IFLYTEK_LOG_PRINT(TEXT("Stop ASR."));
	
	IFlytekASRSocketSubsystem->StopSendAudioData(InHandle);
	
	IFlytekASRSocketSubsystem->CloseSocket();
}

void FIFlytekVoiceManage::StartTTS_ByWebSocket(const FIFlytekTTSInfo& InConfigInfo)
{
	// 创建子系统，如果已经存在则不会创建新的副本
	IFlytekTTSSocketSubsystem = FAITestHelpers::GetWorld()->GetGameInstance()->GetSubsystem<UIFlytekTTSSocketSubsystem>();

	IFlytekTTSSocketSubsystem->CreateSocket(InConfigInfo);
	
	IFlytekTTSSocketSubsystem->SendData(InConfigInfo);
}

void FIFlytekVoiceManage::InitLog()
{
	// 配置初始化
	FIFlytekVoiceConfig::Get()->Init();
}
