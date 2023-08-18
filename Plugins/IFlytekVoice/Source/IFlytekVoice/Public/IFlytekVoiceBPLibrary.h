#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "UObject/Object.h"
#include "IFlytekVoiceBPLibrary.generated.h"

/**
 * 此类为暴露给蓝图的接口
 * 需继承UObject类
 */
UCLASS(meta = (BlueprintThreadSafe, ScripName = "IFlytekVoiceBPLibrary"))
class IFLYTEKVOICE_API UIFlytekVoiceBPLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice")
	static void StartASR_ByWebSocket(const FIFlytekASRInfo& InConfigInfo);
};
