#include "JSON/IFlytekVoiceJson.h"
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
}
