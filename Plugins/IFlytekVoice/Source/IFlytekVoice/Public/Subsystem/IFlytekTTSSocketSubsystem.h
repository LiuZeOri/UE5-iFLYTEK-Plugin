#pragma once

#include "Core/IFlytekVoiceSubsystem.h"
#include "IFlytekVoiceType.h"
#include "IFlytekTTSSocketSubsystem.generated.h"

/**
 * 科大讯飞TTS语音合成（流式版）Socket子系统
 * 官方Web API文档：https://www.xfyun.cn/doc/tts/online_tts/API.html#%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%B5%81%E7%A8%8B
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekTTSSocketSubsystem : public UIFlytekVoiceSubsystem
{
	GENERATED_BODY()

public:
	void CreateSocket(const FIFlytekTTSInfo& InConfigInfo, bool bInAutoPlay = true, bool bInSaveToFile = false, const FString& InFilePath = TEXT(""));
	void SendData(const FString& content, const FIFlytekTTSInfo& InConfigInfo);

protected:
	virtual void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean) override;
	virtual void OnMessage(const FString& Message) override;

private:
	FString GetAfterEncodeText(const FString& InText);

private:
	bool bAutoPlay;
	bool bSaveToFile;
	FString filePath;
	int32 sampleRate;
	TArray<uint8> PCMData;
	TArray<uint8> WAVData;
};
