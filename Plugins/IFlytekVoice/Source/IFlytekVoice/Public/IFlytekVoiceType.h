#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.generated.h"

/**
 * 用户配置结构体
 * 此结构体定义令牌、AppID等
 */
USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FIFlytekUserInfo
{
	GENERATED_USTRUCT_BODY()

	FIFlytekUserInfo();

	// 讯飞开放平台应用ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString appId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiSecret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiKey;
};

/**
 * 实时语音转写结构体
 * 此结构体定义实时语音相关的参数
 * 官方参考文档：https://www.xfyun.cn/doc/asr/rtasr/API.html#%E6%8E%A5%E5%8F%A3%E8%A6%81%E6%B1%82
 */
USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FIFlytekASRInfo
{
	GENERATED_USTRUCT_BODY()

	FIFlytekASRInfo();

	// 请求地址
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	FString serverURL;
};


