#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

namespace IFlytekVoiceJson
{
	// 反序列化实时语音转写ASR返回的Json数据
	IFLYTEKVOICE_API void ASRSocketRespondedToString(const FString& JsonString, FASRSocketResponded& OutResponded);

	// 序列化语音合成TTS上传数据
	IFLYTEKVOICE_API void TTSSocketRequestToJson(const FIFlytekTTSInfo& InParam, FString& OutJsonString);
}
