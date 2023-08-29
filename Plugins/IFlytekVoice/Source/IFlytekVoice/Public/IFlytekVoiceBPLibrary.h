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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category="IFlytekVoice|ASR|WebSocket")
	static void StartASR_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASRInfo& InConfigInfo, FASRSocketTextDelegate InASRSocketTextDelegate);

	// 结束实时语音转写ASR（WebSocket方法）
	UFUNCTION(BlueprintCallable, Category="IFlytekVoice|ASR|WebSocket")
	static void StopASR_ByWebSocket(int32 InHandle);

	// 开始实时语音听写ASD（WebSocket方法）
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category="IFlytekVoice|ASD|WebSocket")
	static void StartASD_ByWebSocket(const UObject* WorldContextObject, int32& OutHandle, const FIFlytekASDInfo& InConfigInfo, FASDSocketTextDelegate InASDSocketTextDelegate);

	/**
	 * 开始语音合成（流式版）TTS（WebSocket方法）
	 * @param content 待合成的文本
	 * @param InConfigInfo 参数配置
	 * @param bAutoPlay 是否自动播放合成音频
	 * @param bSaveToFile 是否保存为本地文件（只支持保存为wav格式）
	 * @param filePath 保存文件路径，如：E:\xxx.wav
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category="IFlytekVoice|TTS|WebSocket")
	static void StartTTS_ByWebSocket(const UObject* WorldContextObject, const FString& content, const FIFlytekTTSInfo& InConfigInfo, bool bAutoPlay = true, bool bSaveToFile = false, const FString& filePath = TEXT(""));

};
