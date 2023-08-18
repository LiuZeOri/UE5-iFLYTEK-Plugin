#pragma once

#include "CoreMinimal.h"
#include "IFlytekVoiceType.generated.h"

UENUM(BlueprintType)
enum class EASRLanguageType : uint8
{
	// 中英混合
	cn,
	// 广东话
	cn_cantonese,
	// 河南话
	cn_henanese,
	// 西南官话（包括四川、重庆、云南、贵州）
	cn_xinanese,
	// 英语
	en,
	// 阿拉伯语
	ar,
	// 德语
	de,
	// 西班牙语
	es,
	// 法语
	fr,
	// 意大利语
	it,
	// 韩语
	ko,
	// 俄语
	ru,
	// 日语
	ja,
	// 越南语
	vi,
};

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
	FString apiKeyASR;
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

	// 服务器协议
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	FString serverProtocol;

	// 实时语音转写语种，不传默认为中文
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	EASRLanguageType Language;

public:
	FString GetLanguageString() const;
};


