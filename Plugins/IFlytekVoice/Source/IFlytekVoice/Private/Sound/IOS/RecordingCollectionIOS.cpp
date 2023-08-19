#include "Sound/IOS/RecordingCollectionIOS.h"

#if defined(__APPLE__) || defined(TARGET_IPHONE_SIMULATOR) ||defined( TARGET_OS_IPHONE) || defined(TARGET_OS_IPHONE)

void FRecordingCollectionIOS::InintConfig(const FRecordingConfig& InConfig)
{

}

bool FRecordingCollectionIOS::StartSpeak()
{
	return false;
}

bool FRecordingCollectionIOS::StopSpeak()
{
	return false;
}

uint8* FRecordingCollectionIOS::GetBuffer(int32& OutSize)
{
	return NULL;
}

void FRecordingCollectionIOS::ClearBuffer()
{

}

bool FRecordingCollectionIOS::IsSpeak()const
{
	return false;
}
#endif // #if __APPLE__ || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || TARGET_OS_IPHONE
