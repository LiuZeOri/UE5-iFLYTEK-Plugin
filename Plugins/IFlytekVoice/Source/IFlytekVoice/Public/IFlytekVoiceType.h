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

UENUM(BlueprintType)
enum class EASRTranslationStrategy : uint8
{
	// 策略1：转写的vad结果直接送去翻译
	strategyOne,
	// 策略2：返回中间过程中的结果（建议）
	strategyTwo,
	// 策略3：按照结束性标点拆分转写结果请求翻译
	strategyThree,
};

UENUM(BlueprintType)
enum class EASRTranslationLanguageType : uint8
{
	// 中文
	cn,
	// 广东话
	cn_cantonese,
	// 英文
	en,
	// 日语
	ja,
	// 韩语
	ko,
	// 俄语
	ru,
	// 法语
	fr,
	// 西班牙语
	es,
	// 越南语
	vi,
};

UENUM(BlueprintType)
enum class EASRPersonalizationParameters : uint8
{
	// 法院
	court,
	// 教育
	edu,
	// 金融
	finance,
	// 医疗
	medical,
	// 科技
	tech,
	// 运营商
	isp,
	// 政府
	gov,
	// 电商
	ecom,
	// 军事
	mil,
	// 企业
	com,
	// 生活
	life,
	// 汽车
	car,
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

	// 是否开启翻译功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	bool bTranslation;

	// 翻译策略
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo", meta=(EditCondition = "bTranslation"))
	EASRTranslationStrategy translationStrategy;

	// 目标翻译语种：控制把源语言转换成什么类型的语言；
	// 请注意类似英文转成法语必须以中文为过渡语言，即英-中-法，暂不支持不含中文语种之间的直接转换
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo", meta=(EditCondition = "bTranslation"))
	EASRTranslationLanguageType translationLanguageType;

	// 是否过滤标点符号
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	bool bFilterPunctuation;

	// 是否启用垂直领域个性化参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	bool bUsePersonalizationParameter;

	// 垂直领域个性化参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo", meta=(EditCondition = "bUsePersonalizationParameter"))
	EASRPersonalizationParameters PersonalizationParameters;

public:
	FString GetLanguageTypeString() const;
	FString GetTranslationStrategyString() const;
	FString GetTranslationLanguageTypeString() const;
	FString GetPersonalizationParameterString() const;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FRecordingConfig
{
	GENERATED_USTRUCT_BODY()

	FRecordingConfig();

	// 采样率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RecordingConfig")
	int32 SampleRate;

	// 录制通道，1单通道，2双通道
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RecordingConfig")
	int32 Channels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RecordingConfig")
	int32 BitsPerSample;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RecordingConfig")
	int32 BufferSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RecordingConfig")
	int32 ExtractDataTime;
};

struct IFLYTEKVOICE_API FWaveHeard
{
	int8 ChunkID[4];
	uint32 ChunkSize;
	int8 Format[4];
};

struct IFLYTEKVOICE_API FWaveFmt
{
	int8 SubChunkID[4];
	uint32 SubChunkSize;
	uint16 AudioFormat;
	uint16 NumChannel;
	uint32 SampleRate;
	uint32 ByteRate;
	uint16 BlockAlign;
	uint16 BitsForSample;
};

struct IFLYTEKVOICE_API FWaveData
{
	int8 DataChunkID[4];
	uint32 DataChunkSize;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FASRSocketResponded
{
	GENERATED_USTRUCT_BODY()

	FASRSocketResponded();

	// 目标语种翻译文本结果，与原始文本src对应
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	FString dst;

	// 翻译结束标识，如果为 true，标识翻译结果已推送完成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	bool isEnd;
	
	// 送翻译的原始文本，音频对应的识别文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	FString src;

	// 结果类型标识，0-最终结果；1-中间结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	int32 type;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FASRSocketTextDelegate, FString, sourceText, FString, translateText);