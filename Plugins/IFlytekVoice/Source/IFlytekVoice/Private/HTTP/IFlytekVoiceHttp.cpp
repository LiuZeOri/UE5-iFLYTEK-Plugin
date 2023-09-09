#include "HTTP/IFlytekVoiceHttp.h"
#include "IFlytekVoiceConfig.h"
#include "JSON/IFlytekVoiceJson.h"

namespace IFlytekVoiceHttp
{
	FHttp::FHttp(FHttpDelegate InDelegate)
		: Delegate(InDelegate)
	{
		bLeaveUnused = true;
	}

	TSharedPtr<FHttp> FHttp::CreateHttpObject(FHttpDelegate InDelegate)
	{
		return MakeShareable(new FHttp(InDelegate));
	}

	bool FHttp::Request(const FString& InURL, const FString& InContent,
		const TMap<FString, FString>& InCustomMetadataHeader, EHttpVerbType VerbType)
	{
		if (bLeaveUnused)
		{
			bLeaveUnused = false;

			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

			// 设置URL
			Request->SetURL(InURL);

			// 设置头
			Request->SetHeader(TEXT("Content-Type"), FString("application/json;charset=UTF-8"));
			for (auto& Tmp : InCustomMetadataHeader)
			{
				Request->SetHeader(Tmp.Key, Tmp.Value);
			}

			// 设置方式
			Request->SetVerb(GetHttpVerbTypeString(VerbType));

			// 设置内容
			Request->SetContentAsString(InContent);

			// 绑定委托
			Request->OnProcessRequestComplete().BindSP(this, &FHttp::OnRequestComplete);

			return Request->ProcessRequest();
		}

		return false;
	}

	bool FHttp::TMRequest(const FString& content, const FIFlytekTMInfo& InConfigInfo, EHttpVerbType VerbType)
	{
		// 配置URL
		// 获取当前时间，yyyy-MM-dd'T'HH:mm:ssZ
		FString utc = FDateTime::Now().UtcNow().ToString(TEXT("%Y-%m-%dT%H:%M:%SZ"));
		
		// 将utc进行urlencode
		utc.ReplaceInline(TEXT(":"), TEXT("%3A"));
		utc.ReplaceInline(TEXT("Z"), TEXT("%2B0000"));

		// 生成uuid
		FString uuid = FGuid::NewGuid().ToString();

		// baseString获取
		FString baseString = FString::Printf(TEXT("accessKeyId=%s&accessKeySecret=%s&appId=%s&utc=%s&uuid=%s"),
			*FIFlytekVoiceConfig::Get()->UserInfo.apiKeyTM,
			*FIFlytekVoiceConfig::Get()->UserInfo.apiSecret,
			*FIFlytekVoiceConfig::Get()->UserInfo.appID,
			*utc,
			*uuid);

		// 根据得到的baseString和APISecret，利用HMAC运算(HmacSHA1)生成签名byte
		// 然后使用base64编码得到signature
		FSHAHash outHash;
		FSHA1::HMACBuffer(TCHAR_TO_ANSI(*FIFlytekVoiceConfig::Get()->UserInfo.apiSecret),
			FIFlytekVoiceConfig::Get()->UserInfo.apiSecret.Len(),
			TCHAR_TO_ANSI(*baseString),
			baseString.Len(),
			outHash.Hash);
		FString signature = FBase64::Encode(outHash.Hash, 20);
		signature.ReplaceInline(TEXT("="), TEXT("%3D"));

		// 拼接URL
		FString URL = FString::Printf(TEXT("%s?accessKeyId=%s&accessKeySecret=%s&utc=%s&signature=%s&appId=%s&uuid=%s"),
			*InConfigInfo.serverURL,
			*FIFlytekVoiceConfig::Get()->UserInfo.apiKeyTM,
			*FIFlytekVoiceConfig::Get()->UserInfo.apiSecret,
			*utc,
			*signature,
			*FIFlytekVoiceConfig::Get()->UserInfo.appID,
			*uuid
			);

		// 配置请求Json
		FString ConfigJsonString;
		IFlytekVoiceJson::TMConfigToJson(InConfigInfo, content, ConfigJsonString);
		
		TMap<FString, FString> InCustomMetadataHeader;
		
		return Request(URL, ConfigJsonString, InCustomMetadataHeader, VerbType);
	}

	void FHttp::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		bLeaveUnused = true;

		Delegate.CompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, bSucceeded);
	}

	FString FHttp::GetHttpVerbTypeString(EHttpVerbType InVerbType)
	{
		switch (InVerbType)
		{
		case POST:
			return TEXT("POST");
		case PUT:
			return TEXT("PUT");
		case GET:
			return TEXT("GET");
		case DELETE:
			return TEXT("DELETE");
		default:
			return TEXT("POST");
		}
	}
}
