#pragma once

// 日志打印打印
#define IFLYTEK_PRINT(Content, CategoryName, ...)									\
{																					\
	FString A_Msg = FString::Printf(Content, __VA_ARGS__);							\
	if (IsInGameThread())															\
	{																				\
		UE_LOG(IFlytekVoiceLog, CategoryName, TEXT("%s"), *A_Msg);					\
	}																				\
	else																			\
	{																				\
		FFunctionGraphTask::CreateAndDispatchWhenReady([A_Msg]()					\
		{																			\
			UE_LOG(IFlytekVoiceLog, CategoryName, TEXT("%s"), *A_Msg);				\
		}, TStatId(), NULL, ENamedThreads::GameThread);								\
	}																				\
}																					

#define IFLYTEK_LOG_PRINT(Content, ...)												\
	IFLYTEK_PRINT(Content, Log, __VA_ARGS__)										

#define IFLYTEK_ERROR_PRINT(Content, ...)											\
	IFLYTEK_PRINT(Content, Error, __VA_ARGS__)									

#define IFLYTEK_WARNING_PRINT(Content, ...)											\
	IFLYTEK_PRINT(Content, Warning, __VA_ARGS__)
