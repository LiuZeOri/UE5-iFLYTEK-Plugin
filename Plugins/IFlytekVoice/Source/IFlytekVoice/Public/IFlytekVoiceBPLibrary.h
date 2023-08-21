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
	// 开始实时语音转写ASR（WebSocket方法）
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate);

	// 结束实时语音转写ASR（WebSocket方法）
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void StopASR_ByWebSocket(int32 InHandle);

};
