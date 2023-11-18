#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SoundPlayerAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSoundPlayerAsyncPin, bool, bCompleted);

UCLASS(BlueprintType, Blueprintable)
class IFLYTEKVOICE_API USoundPlayerAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Audio")
	static USoundPlayerAsync* PlaySoundByFile_Sync(const FString& InFilePath);

	virtual void Activate() override;

protected:
	void PlaySoundByFile_Sync_Thread();

public:
	UPROPERTY(BlueprintAssignable, Category = "IFlytek")
	FSoundPlayerAsyncPin Finished;

	FString FilePath;
};