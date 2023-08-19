#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"

class IFLYTEKVOICE_API FRecordingCollection : public TSharedFromThis<FRecordingCollection>
{
public:
	static TSharedPtr<FRecordingCollection> CreateRecordingCollection();

	virtual void InitConfig(const FRecordingConfig& InConfig) = 0;
	virtual bool StartSpeak() = 0;
	virtual bool StopSpeak() = 0;

	virtual uint8* GetBuffer(int32& OutSize) = 0;
	virtual void ClearBuffer() = 0;

	virtual bool IsSpeak() const = 0;
};
