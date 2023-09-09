#pragma once

#include "CoreMinimal.h"
#include "Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "IWebSocket.h"
#include "IFlytekVoiceSubsystem.generated.h"

/**
 * 科大讯飞子系统基类
 * 所有子系统继承于该类
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */

UCLASS()
class IFLYTEKVOICE_API UIFlytekVoiceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void CloseSocket();
	
	virtual void OnConnected();
	virtual void OnConnectionError(const FString& Error);
	virtual void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	virtual void OnMessage(const FString& Message);
	virtual void OnMessageSent(const FString& MessageString);

public:
	struct FIFlytekAbandonableTask : FNonAbandonableTask
	{
		FIFlytekAbandonableTask(const FSimpleDelegate& InThreadDelegate)
			:ThreadDelegate(InThreadDelegate)
		{
		}

		~FIFlytekAbandonableTask()
		{
		}

		void DoWork()
		{
			ThreadDelegate.ExecuteIfBound();
		}

		// ID
		FORCEINLINE TStatId GetStatId() const
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FASRAbandonable, STATGROUP_ThreadPoolAsyncTasks);
		}

	protected:
		FSimpleDelegate ThreadDelegate;
	};

protected:
	TSharedPtr<IWebSocket> Socket;
};