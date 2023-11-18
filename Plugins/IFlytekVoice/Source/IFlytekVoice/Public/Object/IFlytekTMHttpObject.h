#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.h"
#include "HTTP/IFlytekVoiceHttp.h"
#include "IFlytekTMHttpObject.generated.h"

/**
 * 科大讯飞文本合规TM（TextModeration）Object
 * 项目需求：用于检测文本中不合规的信息
 * 
 * 三个功能：
 *	1、单个请求，输入文本后进行合规检测，返回通过或不通过
 *	2、对讯飞星火（流式返回）返回的内容进行检测，流式返回通过或不通过，并返回是否结束
 *	3、对其他语言模型（一次性返回）返回的内容进行检测，以自定义长度的字符分隔进行检测，流式返回通过或不通过，并返回是否结束
 *	
 * 官方Web API文档：https://www.xfyun.cn/doc/nlp/TextModeration/API.html#%E6%8E%A5%E5%8F%A3%E8%AF%B4%E6%98%8E
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS(BlueprintType, Blueprintable)
class IFLYTEKVOICE_API UIFlytekTMHttpObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIFlytekTMHttpObject", WorldContext = "WorldContextObject"), Category = "IFlytekVoice|TM")
	static UIFlytekTMHttpObject* CreateObject(UObject* WorldContextObject, UClass* InClass = nullptr, UObject* InParent = nullptr, ETMFunction Function = ETMFunction::Single);

public:
	UFUNCTION(BlueprintCallable, Category = "IFlytekVoice|TM")
	void StartTextModeration(const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate);

	UFUNCTION(BlueprintCallable, Category = "IFlytekVoice|TM")
	void StartTextModerationForSparkDesk(UPARAM(ref) TArray<FString>& content, const FIFlytekTMInfo& InConfigInfo, UPARAM(ref) bool& bSparkDeskFinished, FTMHttpForSparkDeskDelegate InTMHttpForSparkDeskDelegate);

	UFUNCTION(BlueprintCallable, Category = "IFlytekVoice|TM")
	void StartTextModerationForOpenAI(const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpForOpenAIDelegate InTMHttpForOpenAIDelegate);

protected:
	void SendRequest_Thread(const FString content, const FIFlytekTMInfo InConfigInfo);
	void SendRequestForSparkDesk_Thread(TArray<FString>& content, const FIFlytekTMInfo InConfigInfo, bool& bSparkDeskFinished);
	void SendRequestForOpenAI_Thread(FString content, const FIFlytekTMInfo InConfigInfo);

protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestForSparkDeskComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnRequestForOpenAIComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,bool bSucceeded);

protected:
	void InitTM(ETMFunction Function);

protected:
	FTMHttpDelegate TMHttpDelegate;
	FTMHttpForSparkDeskDelegate TMHttpForSparkDeskDelegate;
	FTMHttpForOpenAIDelegate TMHttpForOpenAIDelegate;

private:
	TSharedPtr<IFlytekVoiceHttp::FHttp> Http;
	
	FString SubText;
	FString ContentForOpenAI;
	bool bOccupiedForSparkDesk = false;
	bool bOccupiedForOpenAI = false;
	bool bTMForOpenAISucceed = false;
	bool bTMForOpenAIEnd = false;
	int32 index;
};
