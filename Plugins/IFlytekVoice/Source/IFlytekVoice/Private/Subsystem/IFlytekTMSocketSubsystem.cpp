#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "JSON/IFlytekVoiceJson.h"
#include "Subsystem/IFlytekTMHttpSubsystem.h"

void UIFlytekTMHttpSubsystem::SendRequest (const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate)
{
	// 绑定委托
	TMHttpDelegate = InTMHttpDelegate;
	
	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekTMHttpSubsystem::SendRequest_Thread,
		content, InConfigInfo)))->StartBackgroundTask();
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

void UIFlytekTMHttpSubsystem::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);

		if (Response.code.Equals(TEXT("000000")))
		{
			if (Response.suggest.Equals(TEXT("pass")))
			{
				TMHttpDelegate.ExecuteIfBound(true, true);
			}
			else if (Response.suggest.Equals(TEXT("block")))
			{
				TMHttpDelegate.ExecuteIfBound(true, false);
			}
			TMHttpDelegate.Clear();
		}
		else
		{
			TMHttpDelegate.ExecuteIfBound(false, false);
		}
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
	}
}
