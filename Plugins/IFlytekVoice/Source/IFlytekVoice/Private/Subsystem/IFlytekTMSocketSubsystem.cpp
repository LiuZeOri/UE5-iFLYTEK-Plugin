#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "Subsystem/IFlytekTMHttpSubsystem.h"

void UIFlytekTMHttpSubsystem::SendRequest (const FString& content, const FIFlytekTMInfo& InConfigInfo)
{
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
		FString Response = HttpResponse->GetContentAsString();
		IFLYTEK_LOG_PRINT(TEXT("%s"), *Response);
	}
	
}
