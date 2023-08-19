
#include "Sound/RecordingCollection.h"
#include "Sound/Windows/RecordingCollectionWindows.h"
#include "Sound/IOS/RecordingCollectionIOS.h"

class FRecordingCollectionNone : public FRecordingCollection
{
public:
	virtual void InitConfig(const FRecordingConfig& InConfig) {}
	virtual bool StartSpeak(){ return false; }
	virtual bool StopSpeak(){ return false; }
	virtual uint8* GetBuffer(int32& OutSize) { return nullptr;}
	virtual void ClearBuffer() {}
	virtual bool IsSpeak() const { return false; }
};

TSharedPtr<FRecordingCollection> FRecordingCollection::CreateRecordingCollection()
{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
	return MakeShareable<FRecordingCollectionWindows>(new FRecordingCollectionWindows());
#elif defined(__APPLE__) || defined(TARGET_IPHONE_SIMULATOR) ||defined( TARGET_OS_IPHONE) || defined(TARGET_OS_IPHONE)
	return MakeShareable<FRecordingCollectionIOS>(new FRecordingCollectionIOS());
#else	
	return MakeShareable<FRecordingCollection>(new FRecordingCollectionNone());
#endif
}
