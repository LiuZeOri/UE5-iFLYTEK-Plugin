#include "IFlytekVoiceManage.h"

#include "AITestsCommon.h"
#include "IFlytekVoiceConfig.h"
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

void FIFlytekVoiceManage::StartASR_ByWebSocket(const FIFlytekASRInfo& InConfigInfo)
{
	UIFlytekSocketSubsystem* IFlytekSocketSubsystem = FAITestHelpers::GetWorld()->GetGameInstance()->GetSubsystem<UIFlytekSocketSubsystem>();
	
	IFlytekSocketSubsystem->CreateSocket(InConfigInfo);
	
}

void FIFlytekVoiceManage::InitLog()
{
	//配置初始化
	FIFlytekVoiceConfig::Get()->Init();
}
