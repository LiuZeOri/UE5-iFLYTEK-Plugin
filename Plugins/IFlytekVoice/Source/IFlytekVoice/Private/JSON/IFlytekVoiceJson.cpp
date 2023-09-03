#include "JSON/IFlytekVoiceJson.h"

#include "IFlytekVoiceConfig.h"
#include "Json.h"

namespace IFlytekVoiceJson
{
	void ASRSocketRespondedToString(const FString& JsonString, FASRSocketResponded& OutResponded)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonValue> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
			{
				OutResponded.action = InJsonObject->GetStringField(TEXT("action"));
				FString DataString;
				if (InJsonObject->TryGetStringField(TEXT("data"), DataString))
				{
					// 解析 "data" 字符串为 JSON 对象
					TSharedRef<TJsonReader<>> DataReader = TJsonReaderFactory<>::Create(DataString);
					TSharedPtr<FJsonValue> DataRoot;
					if (FJsonSerializer::Deserialize(DataReader, DataRoot))
					{
						if (TSharedPtr<FJsonObject> DataObject = DataRoot->AsObject())
						{
							if (DataObject->GetStringField(TEXT("biz")).Equals(TEXT("trans")))
							{
								OutResponded.dst = DataObject->GetStringField(TEXT("dst"));
								OutResponded.isEnd = DataObject->GetBoolField(TEXT("isEnd"));
								OutResponded.src = DataObject->GetStringField(TEXT("src"));
								OutResponded.type = DataObject->GetIntegerField(TEXT("type"));
								OutResponded.rl = DataObject->GetIntegerField(TEXT("rl"));
							}
						}
					}
				}
			}
		}
	}

	void ASDSocketFirstFrameRequestToJson(const FIFlytekASDInfo& InParam, FString& OutJsonString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);

		JsonWriter->WriteObjectStart();
		{
			// common
			JsonWriter->WriteObjectStart(TEXT("common"));
			{
				JsonWriter->WriteValue(TEXT("app_id"), FIFlytekVoiceConfig::Get()->UserInfo.appID);
			}
			JsonWriter->WriteObjectEnd(); //}

			// business
			JsonWriter->WriteObjectStart(TEXT("business"));
			{
				// 设置语种
				if (InParam.bUseMinorLanguage)
				{
					JsonWriter->WriteValue(TEXT("language"), InParam.minorLanguage);
				}
				else
				{
					JsonWriter->WriteValue(TEXT("language"), InParam.GetLanguageTypeString());
				}

				// 设置应用领域
				JsonWriter->WriteValue(TEXT("domain"), InParam.GetDomainTypeString());

				// 设置方言
				JsonWriter->WriteValue(TEXT("accent"), InParam.accent);

				// 设置静默时间
				if (InParam.bUseVAD)
				{
					JsonWriter->WriteValue(TEXT("vad_eos"), InParam.vad_eos);
				}

				// 设置动态修正
				if (InParam.bUseDwa)
				{
					JsonWriter->WriteValue(TEXT("dwa"), TEXT("wpgs"));
				}

				// 设置领域个性化参数
				if (InParam.bUsePersonalizationParameter)
				{
					JsonWriter->WriteValue(TEXT("pd"), InParam.GetPersonalizationParameterString());
				}

				// 设置是否开启标点符号添加
				if (!InParam.bPunctuation)
				{
					JsonWriter->WriteValue(TEXT("ptt"), 0);
				}

				// 设置字体
				if (InParam.GetLanguageTypeString().Equals(TEXT("zh_cn")))
				{
					JsonWriter->WriteValue(TEXT("rlang"), InParam.GetTypeFaceString());
				}

				// 设置数字格式规则
				if (!InParam.bNunum)
				{
					JsonWriter->WriteValue(TEXT("nunum"), 0);
				}
			}
			JsonWriter->WriteObjectEnd(); //}

			// data
			JsonWriter->WriteObjectStart(TEXT("data"));
			{
				JsonWriter->WriteValue(TEXT("status"), 0);

				// 设置采样率
				JsonWriter->WriteValue(TEXT("format"), InParam.GetAudioFormatString());
				JsonWriter->WriteValue(TEXT("encoding"), TEXT("raw"));
				JsonWriter->WriteValue(TEXT("audio"), TEXT(""));
			}
			JsonWriter->WriteObjectEnd(); //}
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void ASDSocketRequestToJson(const FIFlytekASDInfo& InParam, const FString& audioData, FString& OutJsonString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);

		JsonWriter->WriteObjectStart();
		{
			// data
			JsonWriter->WriteObjectStart(TEXT("data"));
			{
				JsonWriter->WriteValue(TEXT("status"), 1);

				// 设置采样率
				JsonWriter->WriteValue(TEXT("format"), InParam.GetAudioFormatString());
				JsonWriter->WriteValue(TEXT("encoding"), TEXT("raw"));
				JsonWriter->WriteValue(TEXT("audio"), audioData);
			}
			JsonWriter->WriteObjectEnd(); //}
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void ASDSocketLastFrameRequestToJson(const FIFlytekASDInfo& InParam, FString& OutJsonString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);

		JsonWriter->WriteObjectStart();
		{
			// data
			JsonWriter->WriteObjectStart(TEXT("data"));
			{
				JsonWriter->WriteValue(TEXT("status"), 2);
			}
			JsonWriter->WriteObjectEnd(); //}
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void TTSSocketRequestToJson(const FIFlytekTTSInfo& InParam, const FString& content, FString& OutJsonString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString);

		JsonWriter->WriteObjectStart();
		{
			// common
			JsonWriter->WriteObjectStart(TEXT("common"));
			{
				JsonWriter->WriteValue(TEXT("app_id"), FIFlytekVoiceConfig::Get()->UserInfo.appID);
			}
			JsonWriter->WriteObjectEnd(); //}

			// business
			JsonWriter->WriteObjectStart(TEXT("business"));
			{
				// 设置音频编码
				JsonWriter->WriteValue(TEXT("aue"), InParam.GetAudioEncodingTypeString());
				// 需要配合aue=lame使用，开启流式返回mp3格式音频
				if (InParam.GetAudioEncodingTypeString().Equals(TEXT("lame")))
					JsonWriter->WriteValue(TEXT("sfl"), 1);
				// 设置音频采样率
				JsonWriter->WriteValue(TEXT("auf"), InParam.GetAudioSampleRateString());
				// 设置发音人
				JsonWriter->WriteValue(TEXT("vcn"), InParam.vcn);
				// 设置语速
				JsonWriter->WriteValue(TEXT("speed"), InParam.speed);
				// 设置音量
				JsonWriter->WriteValue(TEXT("volume"), InParam.volume);
				// 设置音高
				JsonWriter->WriteValue(TEXT("pitch"), InParam.pitch);
				// 设置是否开启背景音乐
				if (InParam.bgs)
					JsonWriter->WriteValue(TEXT("bgs"), 1);
				// 设置文本编码格式
				JsonWriter->WriteValue(TEXT("tte"), TEXT("UNICODE"));
				// 设置英文发音方式
				if (!InParam.GetEnglishPronunciationTypeString().Equals(TEXT("-1")))
					JsonWriter->WriteValue(TEXT("reg"), InParam.GetEnglishPronunciationTypeString());
				// 设置合成音频数字发音方式
				JsonWriter->WriteValue(TEXT("rdn"), InParam.GetNumberPronunciationTypeString());
			}
			JsonWriter->WriteObjectEnd(); //}

			// data
			JsonWriter->WriteObjectStart(TEXT("data"));
			{
				JsonWriter->WriteValue(TEXT("status"), 2);
				JsonWriter->WriteValue(TEXT("text"), content);
			}
			JsonWriter->WriteObjectEnd(); //}
		}
		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void TTSSocketRespondedToString(const FString& JsonString, FTTSSocketResponded& OutResponded)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonValue> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
			{
				OutResponded.code = InJsonObject->GetIntegerField(TEXT("code"));
				OutResponded.message = InJsonObject->GetStringField(TEXT("message"));

				if (const TSharedPtr<FJsonObject>& DataObject = InJsonObject->GetObjectField(TEXT("data")))
				{
					OutResponded.audio = DataObject->GetStringField(TEXT("audio"));
					OutResponded.status = DataObject->GetIntegerField(TEXT("status"));
				}
			}
		}
	}
}
