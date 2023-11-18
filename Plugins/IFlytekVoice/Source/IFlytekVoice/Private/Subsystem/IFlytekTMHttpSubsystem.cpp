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

void UIFlytekTMHttpSubsystem::SendRequestForOpenAI(const FString& content, const FIFlytekTMInfo& InConfigInfo,
	FTMHttpForOpenAIDelegate InTMHttpForOpenAIDelegate)
{
	// 绑定委托
	TMHttpForOpenAIDelegate = InTMHttpForOpenAIDelegate;
	
	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekTMHttpSubsystem::SendRequestForOpenAI_Thread,
		content, InConfigInfo)))->StartBackgroundTask();
}

void UIFlytekTMHttpSubsystem::SendRequest_Thread(const FString content, const FIFlytekTMInfo InConfigInfo)
{
	// 初始化
	TSharedPtr<IFlytekVoiceHttp::FHttp> Http;
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
		if (!HttpForSparkDesk->IsLeaveUnused() || content.IsEmpty() || bOccupiedForSparkDesk)
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
				
			bOccupiedForSparkDesk = true;
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

void UIFlytekTMHttpSubsystem::SendRequestForOpenAI_Thread(FString content, const FIFlytekTMInfo InConfigInfo)
{
	// 初始化
	TSharedPtr<IFlytekVoiceHttp::FHttp> HttpForOpenAI;
	IFlytekVoiceHttp::FHttpDelegate HttpDelegate;
	HttpDelegate.CompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, bSucceeded);
		});

	HttpForOpenAI = IFlytekVoiceHttp::FHttp::CreateHttpObject(HttpDelegate);

	// 对ChatGPT生成内容进行文本检测
	int32 RemoveLength = 8;
	while(content.Len() > 0)
	{
		if (content.Len() >= 8)
		{
			while (HttpForOpenAI->IsLeaveUnused() || bOccupiedForOpenAI)
            {
				SubText = content.Left(RemoveLength);
				content.LeftChopInline(RemoveLength);
            	HttpForOpenAI->TMRequest(SubText, InConfigInfo);
            	bOccupiedForOpenAI = true;
            }
		}
		else
		{
			while (HttpForOpenAI->IsLeaveUnused() || bOccupiedForOpenAI)
			{
				HttpForOpenAI->TMRequest(content, InConfigInfo);
				bOccupiedForOpenAI = true;
				content.Empty();
				content.Shrink();
			}
		}
	}

	// 检测完成
	IFLYTEK_LOG_PRINT(TEXT("TM for OpenAI Finished."));
	TMHttpForOpenAIDelegate.ExecuteIfBound(TEXT(""), true);
	TMHttpForOpenAIDelegate.Clear();
	HttpForOpenAI.Reset();
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
		IFLYTEK_ERROR_PRINT(TEXT("TM for SparkDesk %s"), *HttpResponse->GetContentAsString())

		if (Response.suggest.Equals(TEXT("pass")))
		{
			TMHttpForSparkDeskDelegate.ExecuteIfBound(index, true, false);
		}
		else if (Response.suggest.Equals(TEXT("block")))
		{
			TMHttpForSparkDeskDelegate.ExecuteIfBound(index, false, false);
		}

		bOccupiedForSparkDesk = false;
		index ++;
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
	}
}

void UIFlytekTMHttpSubsystem::OnRequestForOpenAIComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);
		IFLYTEK_ERROR_PRINT(TEXT("TM for OpenAI %s"), *HttpResponse->GetContentAsString())

		if (Response.suggest.Equals(TEXT("pass")))
		{
			TMHttpForOpenAIDelegate.ExecuteIfBound(SubText, false);
		}
		else if (Response.suggest.Equals(TEXT("block")))
		{
			TMHttpForOpenAIDelegate.ExecuteIfBound(TEXT(""), false);
		}

		bOccupiedForOpenAI = false;
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
	}
}
