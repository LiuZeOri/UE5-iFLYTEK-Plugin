#include "IFlytekVoiceManage.h"

#include "AITestsCommon.h"
#include "IFlytekVoiceConfig.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "Core/IFlytekSocketSubsystem.h"

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

void FIFlytekVoiceManage::InitASRWebSocketSubsystem()
{
	IFlytekSocketSubsystem = FAITestHelpers::GetWorld()->GetGameInstance()->GetSubsystem<UIFlytekSocketSubsystem>();
}

void FIFlytekVoiceManage::StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo)
{
	IFlytekSocketSubsystem->CreateSocket(InConfigInfo);
	
	IFlytekSocketSubsystem->SendAudioData(OutHandle);
}

void FIFlytekVoiceManage::StopASR_ByWebSocket(int32 InHandle)
{
	IFLYTEK_LOG_PRINT(TEXT("Stop ASR."));
	
	IFlytekSocketSubsystem->StopSendAudioData(InHandle);
	
	IFlytekSocketSubsystem->CloseSocket();
}

void FIFlytekVoiceManage::InitLog()
{
	// 配置初始化
	FIFlytekVoiceConfig::Get()->Init();
}
