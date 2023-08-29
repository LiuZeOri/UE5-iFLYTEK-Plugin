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

UENUM(BlueprintType)
enum class EASRFieldType : uint8
{
	// 近场
	nearField,
	// 远场
	farField,
};

UENUM(BlueprintType)
enum class ELanguageRecognitionMode : uint8
{
	// 自动中英文模式
	ChineseAndEnglishMode,
	// 中文模式，可能包含少量英文
	ChineseAndLessEnglishMode,
	// 纯中文模式，不包含英文
	PureChineseMode,
};

UENUM(BlueprintType)
enum class EAudioEncodingType : uint8
{
	// 未压缩的pcm（目前只支持pcm编码，保存为wav文件）
	raw,
	// mp3 (当aue=lame时需传参sfl=1)
	lame,
};

UENUM(BlueprintType)
enum class EAudioSampleRate : uint8
{
	// 合成8K 的音频
	rate8k,
	// 合成16K 的音频
	rate16k,
};

UENUM(BlueprintType)
enum class EEnglishPronunciationType : uint8
{
	// 默认
	defaultMethod,
	// 自动判断处理，如果不确定将按照英文词语拼写处理（缺省）
	methodOne,
	// 所有英文按字母发音
	methodTwo,
	// 自动判断处理，如果不确定将按照字母朗读
	methodThree,
};

UENUM(BlueprintType)
enum class ENumberPronunciationType : uint8
{
	// 自动判断（默认值）
	automaticJudgment,
	// 完全数值
	fullNumericValue,
	// 完全字符串
	fullString,
	// 字符串优先
	stringFirst,
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
	FString appID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiSecret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiKeyASR;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiKeyTTS;
};

/**
 * 实时语音转写参数结构体
 * 此结构体定义实时语音转写相关的参数
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

	// 是否启用远近场切换
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	bool bUseNearOrFarField;

	// 远近场切换
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo", meta=(EditCondition = "bUseNearOrFarField"))
	EASRFieldType FieldType;

	// 是否开角色分离，默认不开启
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	bool bUseRoleSeparation;

	// 语言识别模式选择，默认为模式1，中英文模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASRInfo")
	ELanguageRecognitionMode languageRecognitionMode;

public:
	FString GetLanguageTypeString() const;
	FString GetTranslationStrategyString() const;
	FString GetTranslationLanguageTypeString() const;
	FString GetPersonalizationParameterString() const;
	FString GetFieldType() const;
	FString GetLanguageRecognitionMode() const;
};

/**
 * 实时语音听写参数结构体
 * 此结构体定义实时语音听写相关的参数
 * 官方参考文档：https://www.xfyun.cn/doc/asr/voicedictation/API.html#%E6%8E%A5%E5%8F%A3%E8%AF%B4%E6%98%8E
 */
USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FIFlytekASDInfo
{
	GENERATED_USTRUCT_BODY()

	FIFlytekASDInfo();

	// 请求地址
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString serverURL;

	// 服务器协议
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString serverProtocol;

	// 请求主机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString host;
};

/**
 * 语音合成参数结构体
 * 此结构体定义语音合成的参数
 * 官方参考文档：https://www.xfyun.cn/doc/tts/online_tts/API.html#%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%B5%81%E7%A8%8B
 */
USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FIFlytekTTSInfo
{
	GENERATED_USTRUCT_BODY()

	FIFlytekTTSInfo();

	// 请求地址
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	FString serverURL;

	// 服务器协议
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	FString serverProtocol;

	// 请求主机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	FString host;

	// 音频编码
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	EAudioEncodingType aue;

	// 音频采样率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	EAudioSampleRate auf;

	// 发音人
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	FString vcn;

	// 语速，可选值：[0-100]，默认为50
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	int32 speed;

	// 音量，可选值：[0-100]，默认为50
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	int32 volume;

	// 音高，可选值：[0-100]，默认为50
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	int32 pitch;

	// 是否开启背景声音，默认关闭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	bool bgs;

	// 英文发音方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	EEnglishPronunciationType reg;

	// 合成音频数字发音方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TTSInfo")
	ENumberPronunciationType rdn;

public:
	FString GetAudioEncodingTypeString() const;
	FString GetAudioSampleRateString() const;
	FString GetEnglishPronunciationTypeString() const;
	FString GetNumberPronunciationTypeString() const;
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

	// 结果标识，started:握手，result:结果，error:异常
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	FString action;

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

	// 分离的角色编号，需开启角色分离的功能才返回对应的分离角色编号
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASR|Responded")
	int32 rl;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FTTSSocketResponded
{
	GENERATED_USTRUCT_BODY()

	FTTSSocketResponded();

	// 返回码，0表示成功，其它表示异常
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TTS|Responded")
	int32 code;

	// 结果描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TTS|Responded")
	FString message;

	// 合成后的音频片段，采用base64编码
	FString audio;

	// 结果描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TTS|Responded")
	int32 status;

public:
	TArray<uint8> GetAudioDecodeData() const;
};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FASRSocketTextDelegate, const FASRSocketResponded&, ASRSocketResponded, const FString&, originalText, const FString&, translateText);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FASDSocketTextDelegate, const FASRSocketResponded&, ASDSocketResponded, const FString&, originalText, const FString&, translateText);