#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "IFlytekVoiceType.h"

namespace IFlytekVoiceHttp
{
	enum EHttpVerbType
	{
		POST,
		PUT,
		GET,
		DELETE,
	};

	struct FHttpDelegate
	{
		FHttpRequestCompleteDelegate		CompleteDelegate;
	};

	struct FHttp : public TSharedFromThis<FHttp>
	{
		static TSharedPtr<FHttp> CreateHttpObject (FHttpDelegate InDelegate);
		
	public:
		FHttp(FHttpDelegate InDelegate);

		// 向服务器发送HTTP请求
		bool Request (
			const FString& InURL,
			const FString& InContent,
			const TMap<FString, FString>& InCustomMetadataHeader,
			EHttpVerbType VerbType = EHttpVerbType::POST);

		bool TMRequest (
			const FString& content,
			const FIFlytekTMInfo& InConfigInfo,
			EHttpVerbType VerbType = EHttpVerbType::POST);

	public:
		bool IsLeaveUnused() const { return bLeaveUnused; }

	private:
		void OnRequestComplete (FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	private:
		FString GetHttpVerbTypeString (EHttpVerbType InVerbType);

	private:
		bool bLeaveUnused;	// 是否闲置
		FHttpDelegate Delegate;
	};
}
