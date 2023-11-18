#pragma once

#include "Core/IFlytekVoiceSubsystem.h"
#include "IFlytekVoiceType.h"
#include "HTTP/IFlytekVoiceHttp.h"
#include "IFlytekTMHttpSubsystem.generated.h"

/**
 * 科大讯飞文本合规TM（TextModeration）Http子系统
 * 项目需求：用于检测文本中不合规的信息
 * 官方Web API文档：https://www.xfyun.cn/doc/nlp/TextModeration/API.html#%E6%8E%A5%E5%8F%A3%E8%AF%B4%E6%98%8E
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekTMHttpSubsystem : public UIFlytekVoiceSubsystem
{
	GENERATED_BODY()

public:
	void SendRequest(const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate);
	void SendRequestForSparkDesk(TArray<FString>& content, const FIFlytekTMInfo& InConfigInfo, bool& bSparkDeskFinished, FTMHttpForSparkDeskDelegate InTMHttpForSparkDeskDelegate);
	void SendRequestForOpenAI(const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpForOpenAIDelegate InTMHttpForOpenAIDelegate);
	
	void SendRequest_Thread(const FString content, const FIFlytekTMInfo InConfigInfo);
	void SendRequestForSparkDesk_Thread(TArray<FString>& content, const FIFlytekTMInfo InConfigInfo, bool& bSparkDeskFinished);
	void SendRequestForOpenAI_Thread(FString content, const FIFlytekTMInfo InConfigInfo);

protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestForSparkDeskComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestForOpenAIComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,bool bSucceeded);

protected:
	FTMHttpDelegate TMHttpDelegate;
	FTMHttpForSparkDeskDelegate TMHttpForSparkDeskDelegate;
	FTMHttpForOpenAIDelegate TMHttpForOpenAIDelegate;

public:
	struct FIFlytekParaAbandonableTask : FNonAbandonableTask
	{
		FIFlytekParaAbandonableTask(TFunction<void()>& InThreadFunction)
			: ThreadFunction(InThreadFunction)
		{
		}
    
		~FIFlytekParaAbandonableTask()
		{
		}
    
		void DoWork()
		{
			ThreadFunction();
		}
    
		// ID
		FORCEINLINE TStatId GetStatId() const
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FASRAbandonable, STATGROUP_ThreadPoolAsyncTasks);
		}
    
	protected:
		TFunction<void()> ThreadFunction;
	};

private:
	FString SubText;
	bool bOccupiedForSparkDesk = false;
	bool bOccupiedForOpenAI = false;
	int32 index;
};
