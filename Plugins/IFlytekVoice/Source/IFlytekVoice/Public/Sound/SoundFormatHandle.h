#pragma once

#include "CoreMinimal.h"

namespace SoundFormatHandle
{
	IFLYTEKVOICE_API int32 PCMToWAVByMemory(int32 InChannels, int32 InSampleRate, const TArray<uint8>& InPCMData, TArray<uint8>& OutWAVData);
	
}
