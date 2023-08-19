#pragma once
#if defined(__APPLE__) || defined(TARGET_IPHONE_SIMULATOR) ||defined( TARGET_OS_IPHONE) || defined(TARGET_OS_IPHONE)
#include "Sound/RecordingCollection.h"
#include "AlibabaCloudVoicePlatform.h"

class FRecordingCollectionIOS :public FRecordingCollection
{
public:
	virtual void InintConfig(const FRecordingConfig& InConfig);

	virtual bool StartSpeak();

	virtual bool StopSpeak();

	virtual uint8* GetBuffer(int32& OutSize);

	virtual void ClearBuffer();

	virtual bool IsSpeak() const;

};
#endif