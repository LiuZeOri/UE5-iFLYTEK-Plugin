#pragma once

#include "CoreMinimal.h"

namespace SoundHandle
{
	IFLYTEKVOICE_API bool PlaySoundByFile(const FString& InFilePath);
	IFLYTEKVOICE_API bool PlaySoundByMemory(const uint8* SoundPtr);
	IFLYTEKVOICE_API bool StopSound();
}
