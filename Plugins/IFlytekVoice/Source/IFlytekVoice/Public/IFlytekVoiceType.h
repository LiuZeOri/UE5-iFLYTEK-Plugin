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

UENUM(BlueprintType)
enum class EASDLanguageType : uint8
{
	// 中文（支持简单的英文识别）
	zh_cn,
	// 英文
	en_us,
};

UENUM(BlueprintType)
enum class EASDDomainType : uint8
{
	// 日常用语
	iat,
	// 医疗
	medical,
	// 政务坐席助手
	govSeatAssistant,
	// 金融坐席助手
	seatAssistant,
	// 政务语音分析
	govAnsys,
	// 政务语音导航
	govNav,
	// 金融语音导航
	finNav,
	// 金融语音分析
	finAnsys,
};

UENUM(BlueprintType)
enum class EPersonalizationParameter : uint8
{
	// 游戏
	game,
	// 健康
	health,
	// 购物
	shopping,
	// 旅行
	trip,
};

UENUM(BlueprintType)
enum class ETypeFace : uint8
{
	// 简体中文（默认值）
	zh_cn,
	// 繁体香港
	zh_hk,
};

UENUM(BlueprintType)
enum class EAudioFormat : uint8
{
	// 8K采样率
	rate8k,
	// 16K采样率
	rate16k,
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|UserInfo")
	FString apiKeyTM;
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

	// 请求地址，小语种：ws[s]: //iat-niche-api.xfyun.cn/v2/iat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString serverURL;

	// 服务器协议
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString serverProtocol;

	// 请求主机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString host;

	// 音频采样率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	EAudioFormat format;

	// 语种
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	EASDLanguageType language;

	// 是否使用小语种（使用小语种时请求地址和请求主机需要更改为小语种的）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bUseMinorLanguage;

	// 小语种（填写小语种类型）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo", meta=(EditCondition = "bUseMinorLanguage"))
	FString minorLanguage;

	// 应用领域
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	EASDDomainType domain;

	// 方言（填写方言类型）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	FString accent;

	// 是否启用静默检测，即静默多长时间后引擎认为音频结束。（不启用默认2000ms）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bUseVAD;
	
	// 用于设置端点检测的静默时间，单位是毫秒。
	// 默认2000ms（小语种除外，小语种不设置该参数默认为未开启VAD）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo", meta=(EditCondition = "bUseVAD"))
	int32 vad_eos;

	// 是否启用动态修正（仅中文普通话支持）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bUseDwa;

	// 是否启用领域个性化参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bUsePersonalizationParameter;

	// 领域个性化参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo", meta=(EditCondition = "bUsePersonalizationParameter"))
	EPersonalizationParameter pd;

	// 是否开启标点符号添加（仅中文支持）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bPunctuation;

	// 字体（仅中文支持）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	ETypeFace typeFace;

	// 将返回结果的数字格式规则为阿拉伯数字格式，默认开启（中文普通话和日语支持）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|ASDInfo")
	bool bNunum;
	
public:
	FString GetLanguageTypeString() const;
	FString GetDomainTypeString() const;
	FString GetPersonalizationParameterString() const;
	FString GetTypeFaceString() const;
	FString GetAudioFormatString() const;
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

struct ASDText
{
	ASDText();
	
	FString text;
	FString pgs;
	int32 sn;
	TArray<int32> rg;
	bool deleted;
};

struct ASDDecoder
{
public:
	ASDDecoder();
	void TextDecode(ASDText text);
	void TextDiscard();
	FString OutTextString();
	
private:
	TArray<ASDText> texts;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FASDSocketRespondedWs
{
	GENERATED_USTRUCT_BODY()

	// 字词
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result|Ws")
	TArray<FString> cw;
};


USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FASDSocketRespondedResult
{
	GENERATED_USTRUCT_BODY()

	FASDSocketRespondedResult();

	// 返回结果的序号
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result")
	int32 sn;
	
	// 是否是最后一片结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result")
	bool ls;

	// 开启wpgs会有此字段
	// 取值为 "apd"时表示该片结果是追加到前面的最终结果；取值为"rpl" 时表示替换前面的部分结果，替换范围为rg字段
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result")
	FString pgs;

	// 替换范围，开启wpgs会有此字段
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result")
	TArray<int32> rg;

	// 听写结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data|Result")
	TArray<FASDSocketRespondedWs> ws;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FASDSocketRespondedData
{
	GENERATED_USTRUCT_BODY()

	// 识别结果是否结束标识：
	// 0：识别的第一块结果
	// 1：识别中间结果
	// 2：识别最后一块结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data")
	int32 status;

	// 听写识别结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded|Data")
	FASDSocketRespondedResult result;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FASDSocketResponded
{
	GENERATED_USTRUCT_BODY()

	// 返回码，0表示成功，其它表示异常
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded")
	int32 code;

	// 听写结果信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASD|Responded")
	FASDSocketRespondedData data;
};

/**
 * 文本合规TM（TextModeration）参数结构体
 * 此结构体定义语音合成的参数
 * 官方参考文档：https://www.xfyun.cn/doc/nlp/TextModeration/API.html#%E6%8E%A5%E5%8F%A3demo
 */
USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FIFlytekTMInfo
{
	GENERATED_USTRUCT_BODY()

	FIFlytekTMInfo();

	// 请求地址
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TMInfo")
	FString serverURL;

	// 是否全匹配，默认否，匹配到敏感词则不再匹配，不会返回所有敏感分类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IFlytek|TMInfo")
	bool is_match_all;
};

USTRUCT(BlueprintType)
struct IFLYTEKVOICE_API FTMResponded
{
	GENERATED_USTRUCT_BODY()

	// 会话调用成功标记：000000 表示调用成，其他数字表示会话调用异常
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TM|Responded")
	FString code;

	// 审核建议结果：pass 通过  block 不合规结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TM|Responded")
	FString suggest;
};


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FASRSocketTextDelegate, const FASRSocketResponded&, ASRSocketResponded, const FString&, originalText, const FString&, translateText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FASDSocketTextDelegate, bool, bFinished, const FString&, Text);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FTMHttpDelegate, bool, bSuccessed, bool, bPass);