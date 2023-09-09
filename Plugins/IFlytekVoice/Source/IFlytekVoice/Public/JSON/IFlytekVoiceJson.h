#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

namespace IFlytekVoiceJson
{
	// 反序列化实时语音转写ASR返回的Json数据
	IFLYTEKVOICE_API void ASRSocketRespondedToString(const FString& JsonString, FASRSocketResponded& OutResponded);

	// 序列化实时语音听写ASD上传数据 第一帧
	IFLYTEKVOICE_API void ASDSocketFirstFrameRequestToJson(const FIFlytekASDInfo& InParam, FString& OutJsonString);

	// 序列化实时语音听写ASD上传数据
	IFLYTEKVOICE_API void ASDSocketRequestToJson(const FIFlytekASDInfo& InParam, const FString& audioData, FString& OutJsonString);

	// 序列化实时语音听写ASD上传数据 最后一帧
	IFLYTEKVOICE_API void ASDSocketLastFrameRequestToJson(const FIFlytekASDInfo& InParam, FString& OutJsonString);

	// 反序列化实时语音转写ASD返回的Json数据
	IFLYTEKVOICE_API void ASDSocketRespondedToString(const FString& JsonString, FASDSocketResponded& OutResponded);

	// 序列化语音合成TTS上传数据
	IFLYTEKVOICE_API void TTSSocketRequestToJson(const FIFlytekTTSInfo& InParam, const FString& content, FString& OutJsonString);
	
	// 反序列化语音合成TTS返回的Json数据
	IFLYTEKVOICE_API void TTSSocketRespondedToString(const FString& JsonString, FTTSSocketResponded& OutResponded);

	// 序列化文本合规TM上传参数
	IFLYTEKVOICE_API void TMConfigToJson(const FIFlytekTMInfo& InParam, const FString& content, FString& OutJsonString);

	// 反序列化文本合规TM返回的Json数据
	IFLYTEKVOICE_API void TMRespondedToString(const FString& JsonString, FTMResponded& OutResponded);
}
