#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

/**
 * 功能实现类，为单例类
 */
class IFLYTEKVOICE_API FIFlytekVoiceManage
{
public:
	FIFlytekVoiceManage();
	~FIFlytekVoiceManage();
	
	static FIFlytekVoiceManage* Get();
	static void Destroy();

	void StartASR_ByWebSocket(const FIFlytekASRInfo& InConfigInfo);

protected:
	void InitLog();

private:
	static FIFlytekVoiceManage* Manage;
};
