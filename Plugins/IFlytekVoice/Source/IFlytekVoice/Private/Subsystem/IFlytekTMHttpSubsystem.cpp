#include "Subsystem/IFlytekTMHttpSubsystem.h"
#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "JSON/IFlytekVoiceJson.h"

void UIFlytekTMHttpSubsystem::SendRequest (const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate)
{
	// 绑定委托
	TMHttpDelegate = InTMHttpDelegate;
	
	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekTMHttpSubsystem::SendRequest_Thread,
		content, InConfigInfo)))->StartBackgroundTask();
}

void UIFlytekTMHttpSubsystem::SendRequestForSparkDesk (TArray<FString>& content, const FIFlytekTMInfo& InConfigInfo, bool& bSparkDeskFinished, FTMHttpForSparkDeskDelegate InTMHttpForSparkDeskDelegate)
{
	// 绑定委托
	TMHttpForSparkDeskDelegate = InTMHttpForSparkDeskDelegate;

	// 使用 Lambda 函数创建 TFunction<void()>
	TFunction<void()> SendRequestForSparkDeskTaskFunction(
		[this, &content, InConfigInfo, &bSparkDeskFinished]()
			{
				UIFlytekTMHttpSubsystem::SendRequestForSparkDesk_Thread(content, InConfigInfo, bSparkDeskFinished);
			});

	// 使用 TFunction<void()> 创建 FAutoDeleteAsyncTask
	(new FAutoDeleteAsyncTask<FIFlytekParaAbandonableTask>(SendRequestForSparkDeskTaskFunction))->StartBackgroundTask();

}

void UIFlytekTMHttpSubsystem::SendRequest_Thread(const FString content, const FIFlytekTMInfo InConfigInfo)
{
	// 初始化
	IFlytekVoiceHttp::FHttpDelegate HttpDelegate;
	HttpDelegate.CompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, bSucceeded);
		});

	Http = IFlytekVoiceHttp::FHttp::CreateHttpObject(HttpDelegate);

	// 发送请求
	Http->TMRequest(content, InConfigInfo);
}

void UIFlytekTMHttpSubsystem::SendRequestForSparkDesk_Thread(TArray<FString>& content, const FIFlytekTMInfo InConfigInfo, bool& bSparkDeskFinished)
{
	// 初始化
	TSharedPtr<IFlytekVoiceHttp::FHttp> HttpForSparkDesk;
	IFlytekVoiceHttp::FHttpDelegate HttpDelegate;
	HttpDelegate.CompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestForSparkDeskComplete(HttpRequest, HttpResponse, bSucceeded);
		});
	HttpForSparkDesk = IFlytekVoiceHttp::FHttp::CreateHttpObject(HttpDelegate);
	
	// 开始对星火大模型返回的文本检测
	index = 0;
	while (true)
	{
		if (!HttpForSparkDesk->IsLeaveUnused() || content.IsEmpty() || bOccupied)
		{
			continue;
		}
		
		if (index < content.Num())
		{
			if (!content.IsValidIndex(index))
			{
				continue;
			}
			
			IFLYTEK_LOG_PRINT(TEXT("content number: %i"), content.Num());
			IFLYTEK_LOG_PRINT(TEXT("index: %i"), index);
			IFLYTEK_LOG_PRINT(TEXT("content text: %s"), *content[index]);
			
			// 发送请求
			HttpForSparkDesk->TMRequest(content[index], InConfigInfo);
				
			bOccupied = true;
		}
		else if (bSparkDeskFinished)
		{
			IFLYTEK_LOG_PRINT(TEXT("TM for Spark Desk Finished."));
			TMHttpForSparkDeskDelegate.ExecuteIfBound(0, false, true);
			TMHttpForSparkDeskDelegate.Clear();
			HttpForSparkDesk.Reset();
			break;
		}
	}
}

void UIFlytekTMHttpSubsystem::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);
		IFLYTEK_LOG_PRINT(TEXT("TM %s"), *HttpResponse->GetContentAsString())
		
		if (Response.suggest.Equals(TEXT("pass")))
		{
			TMHttpDelegate.ExecuteIfBound(true);
		}
		else if (Response.suggest.Equals(TEXT("block")))
		{
			TMHttpDelegate.ExecuteIfBound(false);
		}
		TMHttpDelegate.Clear();
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
	}
}

void UIFlytekTMHttpSubsystem::OnRequestForSparkDeskComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);
		IFLYTEK_ERROR_PRINT(TEXT("TM %s"), *HttpResponse->GetContentAsString())

		if (Response.suggest.Equals(TEXT("pass")))
		{
			TMHttpForSparkDeskDelegate.ExecuteIfBound(index, true, false);
		}
		else if (Response.suggest.Equals(TEXT("block")))
		{
			TMHttpForSparkDeskDelegate.ExecuteIfBound(index, false, false);
		}

		bOccupied = false;
		index ++;
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
	}
}
