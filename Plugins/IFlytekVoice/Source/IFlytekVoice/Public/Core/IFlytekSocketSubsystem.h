#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "IWebSocket.h"
#include "Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "IFlytekSocketSubsystem.generated.h"

/**
 * 科大讯飞Socket子系统
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void CreateSocket(const FIFlytekASRInfo& InConfigInfo);
	void CloseSocket();

protected:
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& MessageString);

protected:
	TSharedPtr<IWebSocket> Socket = nullptr;
	FString serverProtocol = TEXT("");

};
