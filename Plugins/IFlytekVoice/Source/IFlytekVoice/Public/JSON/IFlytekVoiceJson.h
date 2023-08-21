#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

namespace IFlytekVoiceJson
{
	// 反序列化实时语音转写ASR返回的Json数据
	IFLYTEKVOICE_API void ASRSocketRespondedToString(const FString& JsonString, FASRSocketResponded& OutResponded);
}
