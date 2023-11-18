#include "Object/IFlytekTMHttpObject.h"

#include "IFlytekVoiceLog.h"
#include "IFlytekVoiceMacro.h"
#include "IFlytekVoiceManage.h"
#include "JSON/IFlytekVoiceJson.h"

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
		RETURN_QUICK_DECLARE_CYCLE_STAT(FIFlytekAbandonableTask, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	FSimpleDelegate ThreadDelegate;
};

UIFlytekTMHttpObject* UIFlytekTMHttpObject::CreateObject(UObject* WorldContextObject, UClass* InClass, UObject* InParent, ETMFunction Function)
{
	check(WorldContextObject != NULL);

	// 配置初始化
	FIFlytekVoiceManage::Get();
	
	UClass* MyClass = InClass;
	if (!InClass)
	{
		MyClass = UIFlytekTMHttpObject::StaticClass();
	}

	if (InParent)
	{
		if (UIFlytekTMHttpObject* Obj = NewObject<UIFlytekTMHttpObject>(InParent, MyClass))
		{
			Obj->InitTM(Function);
			return Obj;
		}
	}
	else
	{
		if (UIFlytekTMHttpObject* Obj = NewObject<UIFlytekTMHttpObject>(WorldContextObject, MyClass))
		{
			Obj->InitTM(Function);
			return Obj;
		}
	}

	return nullptr;
}

void UIFlytekTMHttpObject::StartTextModeration (const FString& content, const FIFlytekTMInfo& InConfigInfo, FTMHttpDelegate InTMHttpDelegate)
{
	// 绑定委托
	TMHttpDelegate = InTMHttpDelegate;
	
	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekTMHttpObject::SendRequest_Thread,
		content, InConfigInfo)))->StartBackgroundTask();
}

void UIFlytekTMHttpObject::StartTextModerationForSparkDesk (UPARAM(ref) TArray<FString>& content, const FIFlytekTMInfo& InConfigInfo, UPARAM(ref) bool& bSparkDeskFinished, FTMHttpForSparkDeskDelegate InTMHttpForSparkDeskDelegate)
{
	// 绑定委托
	TMHttpForSparkDeskDelegate = InTMHttpForSparkDeskDelegate;

	// 使用 Lambda 函数创建 TFunction<void()>
	TFunction<void()> SendRequestForSparkDeskTaskFunction(
		[this, &content, InConfigInfo, &bSparkDeskFinished]()
			{
				UIFlytekTMHttpObject::SendRequestForSparkDesk_Thread(content, InConfigInfo, bSparkDeskFinished);
			});

	// 使用 TFunction<void()> 创建 FAutoDeleteAsyncTask
	(new FAutoDeleteAsyncTask<FIFlytekParaAbandonableTask>(SendRequestForSparkDeskTaskFunction))->StartBackgroundTask();

}

void UIFlytekTMHttpObject::StartTextModerationForOpenAI(const FString& content, const FIFlytekTMInfo& InConfigInfo,
	FTMHttpForOpenAIDelegate InTMHttpForOpenAIDelegate)
{
	// 绑定委托
	TMHttpForOpenAIDelegate = InTMHttpForOpenAIDelegate;
	
	(new FAutoDeleteAsyncTask<FIFlytekAbandonableTask>(
		FSimpleDelegate::CreateUObject(this, &UIFlytekTMHttpObject::SendRequestForOpenAI_Thread,
		content, InConfigInfo)))->StartBackgroundTask();
}

void UIFlytekTMHttpObject::SendRequest_Thread(const FString content, const FIFlytekTMInfo InConfigInfo)
{
	// 发送请求
	Http->TMRequest(content, InConfigInfo);
}

void UIFlytekTMHttpObject::SendRequestForSparkDesk_Thread(TArray<FString>& content, const FIFlytekTMInfo InConfigInfo, bool& bSparkDeskFinished)
{
	// 开始对星火大模型返回的文本检测
	index = 0;
	while (true)
	{
		if (content.IsEmpty() || bOccupiedForSparkDesk || !Http)
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
			if (Http->TMRequest(content[index], InConfigInfo))
			{
				bOccupiedForSparkDesk = true;
			}
			else
			{
				IFLYTEK_ERROR_PRINT(TEXT("TM Request for Spark Desk failed, retry."));
				Http.Reset();
				InitTM(ETMFunction::ForSparkDesk);
				continue;
			}
		}
		else if (bSparkDeskFinished)
		{
			IFLYTEK_LOG_PRINT(TEXT("TM for Spark Desk Finished."));
			TMHttpForSparkDeskDelegate.ExecuteIfBound(0, false, true);
			TMHttpForSparkDeskDelegate.Clear();
			Http.Reset();
			break;
		}
	}
}

void UIFlytekTMHttpObject::SendRequestForOpenAI_Thread(FString content, const FIFlytekTMInfo InConfigInfo)
{
	// 对ChatGPT生成内容进行文本检测
	
	/*
	int32 RemoveLength = 8;
	while(true)
	{
		if (bOccupiedForOpenAI || !Http)
		{
			continue;
		}

		if (bTMForOpenAISucceed)
		{
			if (content.Len() >= RemoveLength)
			{
				content.RightChopInline(RemoveLength);
			}
			else if (content.Len() > 0)
			{
				content.Empty();
				content.Shrink();
			}
			
			if (content.IsEmpty())
			{
				// 检测完成
				IFLYTEK_LOG_PRINT(TEXT("TM for OpenAI Finished."));
				TMHttpForOpenAIDelegate.ExecuteIfBound(TEXT(""), true);
				TMHttpForOpenAIDelegate.Clear();
				Http.Reset();
				bTMForOpenAISucceed = false;
				break;
			}
		}
		
		if (content.Len() >= RemoveLength)
		{
			SubText = content.Left(RemoveLength);
		}
		else if (content.Len() > 0)
		{
			SubText = content;
		}
		Http->TMRequest(SubText, InConfigInfo);
		bOccupiedForOpenAI = true;
	}
	*/

	// 以一句话拆分检测方式
	// 创建分割符
	ContentForOpenAI = content;
	
	const TCHAR* FG[] =
	{
		TEXT(" "),
		TEXT("\n"),
		TEXT("\r"),
		TEXT("\t"),
		TEXT("。"),
		TEXT("，"),
		TEXT("！"),
		TEXT("？"),
		TEXT("、"),
		TEXT("."),
		TEXT(",")
	};
	
	// 计算分隔符的数量
	int32 Number = UE_ARRAY_COUNT(FG);
	TArray<FString> ContentArray;
	
	// 以分隔符切割，给到容器中
	content.ParseIntoArray(ContentArray, FG, Number);

	// 开始文本合规检测
	/*
	for (auto& Tmp : ContentArray)
	{
		SubText = Tmp;
		Http->TMRequest(SubText, InConfigInfo);
		bOccupiedForOpenAI = true;

		while (true)
		{
			if (!bOccupiedForOpenAI && Http)
			{
				break;
			}
		}
	}

	TMHttpForOpenAIDelegate.ExecuteIfBound(ContentForOpenAI, true);
	IFLYTEK_LOG_PRINT(TEXT("TM for OpenAI Finished."));
	TMHttpForOpenAIDelegate.Clear();
	Http.Reset();
	*/

	index = 0;
	while (true)
	{
		if (bOccupiedForOpenAI || !Http)
		{
			continue;
		}
		
		if (index < ContentArray.Num())
		{
			if (index == ContentArray.Num() - 1)
				bTMForOpenAIEnd = true;
			
			// 发送请求
			SubText = ContentArray[index];
			if (Http->TMRequest(SubText, InConfigInfo))
			{
				bOccupiedForOpenAI = true;
			}
			else
			{
				IFLYTEK_ERROR_PRINT(TEXT("TM Request for OpenAI failed, retry."));
				Http.Reset();
				InitTM(ETMFunction::ForOpenAI);
				continue;
			}
		}
		else
		{
			IFLYTEK_LOG_PRINT(TEXT("TM for OpenAI Finished."));
			TMHttpForOpenAIDelegate.Clear();
			Http.Reset();
			break;
		}
	}
}

void UIFlytekTMHttpObject::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
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

void UIFlytekTMHttpObject::OnRequestForSparkDeskComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);
		IFLYTEK_LOG_PRINT(TEXT("TM for SparkDesk %s"), *HttpResponse->GetContentAsString())

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
		bOccupiedForSparkDesk = false;
	}
}

void UIFlytekTMHttpObject::OnRequestForOpenAIComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bSucceeded)
{
	if (bSucceeded)
	{
		FTMResponded Response;
		IFlytekVoiceJson::TMRespondedToString(HttpResponse->GetContentAsString(), Response);
		IFLYTEK_LOG_PRINT(TEXT("TM for OpenAI %s"), *HttpResponse->GetContentAsString())

		/*
		if (Response.suggest.Equals(TEXT("pass")))
		{
			TMHttpForOpenAIDelegate.ExecuteIfBound(SubText, false);
		}
		else if (Response.suggest.Equals(TEXT("block")))
		{
			TMHttpForOpenAIDelegate.ExecuteIfBound(TEXT(""), false);
		}
		*/

		if (Response.suggest.Equals(TEXT("block")))
		{
			ContentForOpenAI.ReplaceInline(*SubText, TEXT(""));
		}

		if (bTMForOpenAIEnd)
		{
			TMHttpForOpenAIDelegate.ExecuteIfBound(ContentForOpenAI, true);
		}

		bOccupiedForOpenAI = false;
		index ++;
		//bTMForOpenAISucceed = true;
	}
	else
	{
		IFLYTEK_ERROR_PRINT(TEXT("TM Http Request error."))
		bOccupiedForOpenAI = false;
		//bTMForOpenAISucceed = false;
	}
}

void UIFlytekTMHttpObject::InitTM(ETMFunction Function)
{
	IFlytekVoiceHttp::FHttpDelegate HttpDelegate;
	
	switch (Function)
	{
	case ETMFunction::Single:
		HttpDelegate.CompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, bSucceeded);
		});
		break;
		
	case ETMFunction::ForSparkDesk:
        HttpDelegate.CompleteDelegate.BindLambda(
        [&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
        {
        	OnRequestForSparkDeskComplete(HttpRequest, HttpResponse, bSucceeded);
        });
		break;
		
	case ETMFunction::ForOpenAI:
		HttpDelegate.CompleteDelegate.BindLambda(
		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
		{
			OnRequestForOpenAIComplete(HttpRequest, HttpResponse, bSucceeded);
		});
		break;
	}

	Http = IFlytekVoiceHttp::FHttp::CreateHttpObject(HttpDelegate);
}
