#pragma once

#include "Core/IFlytekVoiceSubsystem.h"
#include "IFlytekVoiceType.h"
#include "IFlytekSDSocketSubsystem.generated.h"

/**
 * 科大讯飞星火认知大模型（SparkDesk）Socket子系统
 * SparkDesk官网：https://xinghuo.xfyun.cn/
 * 官方Web API文档：https://www.xfyun.cn/doc/spark/Web.html
 * 作者开发文档：https://www.yuque.com/u28988421/ad9c7i/sa1fatpyzrg79994#VbRPQ
 */
UCLASS()
class IFLYTEKVOICE_API UIFlytekSDSocketSubsystem : public UIFlytekVoiceSubsystem
{
	GENERATED_BODY()
public:
	void CreateSocket(const FIFlytekSDInfo& InConfigInfo, FSDSocketDelegate InSDSocketDelegate);
	void SendData(const FString& content, const FIFlytekSDInfo& InConfigInfo);

protected:
	virtual void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean) override;
	virtual void OnMessage(const FString& Message) override;

protected:
	FSDSocketDelegate SDSocketDelegate;
};