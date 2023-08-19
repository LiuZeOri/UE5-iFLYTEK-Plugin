#pragma once

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
#include "Sound/RecordingCollection.h"
#include "IFlytekVoicePlatform.h"
#include <mmreg.h>
#include <MSAcm.h>

class FRecordingCollectionWindows : public FRecordingCollection
{
public:
	FRecordingCollectionWindows();
	
	virtual void InitConfig(const FRecordingConfig& InConfig) override;
	virtual bool StartSpeak() override;
	virtual bool StopSpeak() override;

	virtual uint8* GetBuffer(int32& OutSize) override;
	virtual void ClearBuffer() override;

	virtual bool IsSpeak() const override;

protected:
	WAVEFORMATEX WaveformatSetting;	// 采集音频格式
	HANDLE EvenHandle;
	HWAVEIN Wavein;
	bool bSpeak;
	uint8* Buffer;
	int32 BufferSize;
	WAVEHDR Wavehdr;

	int32 ExtractDataTime;
};
#endif
