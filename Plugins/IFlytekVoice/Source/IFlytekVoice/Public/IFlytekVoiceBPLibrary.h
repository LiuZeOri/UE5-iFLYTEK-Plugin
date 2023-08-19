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
	// 初始化实时语音转写WebSocket子系统，只需要初始化一次
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void InitASRWebSocketSubsystem();

	// 开始实时语音转写（WebSocket方法）
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void StartASR_ByWebSocket(int32& OutHandle, const FIFlytekASRInfo& InConfigInfo);

	// 结束实时语音转写（WebSocket方法）
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void StopASR_ByWebSocket(int32 InHandle);

};
