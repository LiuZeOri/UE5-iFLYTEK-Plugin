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

// hmac_sha256编码，并进行base64加密
#define HMACSHA256 [](FString apiSecreet, FString data)->FString					\
{																					\
	uint8_t md[SHA256_DIGESTLEN] = { 0 };											\
	uint8_t* secret = (uint8_t*)TCHAR_TO_UTF8(*apiSecreet);							\
	uint8_t* indata = (uint8_t*)TCHAR_TO_UTF8(*data);								\
	HMAC_SHA256_CTX hmac;															\
	hmac_sha256_init(&hmac, secret, strlen((char*)secret));							\
	hmac_sha256_update(&hmac, indata, strlen((char*)indata));						\
	hmac_sha256_final(&hmac, md);													\
	return FBase64::Encode(md, SHA256_DIGESTLEN);									\
}

// 发送字节
#define FRAME_SIZE 1280
