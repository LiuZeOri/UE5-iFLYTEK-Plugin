#include "Sound/SoundPlayerAsync.h"
#include "Sound/SoundHandle.h"

struct FSoundPlayerTask : FNonAbandonableTask
{
	FSoundPlayerTask(const FSimpleDelegate& InThreadDelegate)
		:ThreadDelegate(InThreadDelegate)
	{
	}

	~FSoundPlayerTask()
	{
	}

	void DoWork()
	{
		ThreadDelegate.ExecuteIfBound();
	}

	// ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSoundPlayerTask, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	FSimpleDelegate ThreadDelegate;
};

USoundPlayerAsync* USoundPlayerAsync::PlaySoundByFile_Sync(const FString& InFilePath)
{
	USoundPlayerAsync* BPNode = NewObject<USoundPlayerAsync>();
	BPNode->FilePath = InFilePath;
	return BPNode;
}

void USoundPlayerAsync::Activate()
{
	(new FAutoDeleteAsyncTask<FSoundPlayerTask>(
		FSimpleDelegate::CreateUObject(this, &USoundPlayerAsync::PlaySoundByFile_Sync_Thread)))->StartBackgroundTask();
}

void USoundPlayerAsync::PlaySoundByFile_Sync_Thread()
{
	SoundHandle::PlaySoundByFile_SYNC(FilePath);
	Finished.Broadcast(true);
}
