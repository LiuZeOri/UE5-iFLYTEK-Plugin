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
	void SendRequest (const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate);
	void SendRequest_Thread (const FString content, const FIFlytekTMInfo InConfigInfo);

protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

protected:
	FTMHttpDelegate TMHttpDelegate;

private:
	TSharedPtr<IFlytekVoiceHttp::FHttp> Http;
};
