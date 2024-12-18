#include "XGSampleBDReqType.h"

bool FXGSampleBDReqUti::WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, const FString& InStringValue)
{
	if (InStringValue.IsEmpty() || InStringValue.Equals(TEXT("none")) || InStringValue.Equals(TEXT("None")))
	{

		return false;

	}
	ReqJsonWriter->WriteValue(InName, InStringValue);


	return true;
}

bool FXGSampleBDReqUti::WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, int32 InIntValue)
{
	if (InIntValue == -1)
	{
		return false;
	}

	ReqJsonWriter->WriteValue(InName, InIntValue);

	return true;
}

bool FXGSampleBDReqUti::WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, float InFloatValue)
{
	if (InFloatValue == -1.f)
	{
		return false;
	}

	ReqJsonWriter->WriteValue(InName, InFloatValue);

	return true;
}

bool FXGSampleBDReqUti::WriteJsonValue(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter, const FString& InName, bool InBoolValue)
{
	ReqJsonWriter->WriteValue(InName, InBoolValue);

	return true;
}



void FXGSampleBDReqMessageInfo::ToJsonWriter(TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter)
{
	ReqJsonWriter->WriteObjectStart();

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("role"), role);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("content"), content);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("name"), name);



	ReqJsonWriter->WriteObjectEnd();
}


FString FXGSampleBDReqInfo::ToJsonString()
{
	FString ReqJsonString = TEXT("");

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ReqJsonWriter
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ReqJsonString);
	ReqJsonWriter->WriteObjectStart();


	ReqJsonWriter->WriteArrayStart(TEXT("messages"));
	for (auto& Tmp : messages)
	{
		Tmp.ToJsonWriter(ReqJsonWriter);
	}
	ReqJsonWriter->WriteArrayEnd();

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("temperature"), temperature);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("top_p"), top_p);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("penalty_score"), penalty_score);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("stream"), stream);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("system"), system);

	if (stop.Num() > 0)
	{
		ReqJsonWriter->WriteArrayStart(TEXT("stop"));
		for (auto& Tmp : stop)
		{
			if (Tmp.IsEmpty())
			{
				continue;
			}
			ReqJsonWriter->WriteValue(Tmp);

		}

		ReqJsonWriter->WriteArrayEnd();
	}

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("disable_search"), disable_search);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("enable_citation"), enable_citation);
	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("enable_trace"), enable_trace);

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("max_output_tokens"), max_output_tokens);

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("response_format"), response_format);

	FXGSampleBDReqUti::WriteJsonValue(ReqJsonWriter, TEXT("user_id"), user_id);




	ReqJsonWriter->WriteObjectEnd();
	ReqJsonWriter->Close();

    return ReqJsonString;
}

