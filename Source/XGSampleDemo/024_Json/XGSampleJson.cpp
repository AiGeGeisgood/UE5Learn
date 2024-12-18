
#include "XGSampleJson.h"
#include "Misc/FileHelper.h"


#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"

#include "JsonObjectConverter.h"

// Sets default values
AXGSampleJson::AXGSampleJson()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AXGSampleJson::NotGoodJson()
{
	/*
	{
		"Code": 0,
		"Message": "Success",
		"Data": "alksjdoiqwjdnlknasklcjklasjdklj",
		"Sid": "19876678"
	}

	*/

	TempMessageInfo MyTempInfo;
	MyTempInfo.UECode = 998;
	MyTempInfo.UEMessage = TEXT("XGTest");
	MyTempInfo.UEData = TEXT("123455");
	MyTempInfo.Sid = TEXT("11111");

	FString NotGooDJson = FString::Printf(TEXT("{\"Code\":%d,\"Message\": \"%s\",\"Data\": \"%s\",\"Sid\" : \"%s\"}"),
		MyTempInfo.UECode, *MyTempInfo.UEMessage, *MyTempInfo.UEData, *MyTempInfo.Sid);

	FString FilePath = FPaths::ProjectSavedDir() / TEXT("NoGoodJson.json");
	//序列化
	FFileHelper::SaveStringToFile(NotGooDJson, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	//反序列化

	FString BackNotGoodJson = TEXT("");

	FFileHelper::LoadFileToString(BackNotGoodJson, *FilePath);

	//后续切割太麻烦.

	int32 a = 1;


}

void AXGSampleJson::GoodJson()
{
	/*
	{
		"Code" : 0,
		"Message" : "Success",
		"Data": "alksjdoiqwjdnlknasklcjklasjdklj",
		"Sid": "19876678"
		"Info":
				{
					"ServerName": "XG",
					"ServerVersion" : 19356
				}
	}

	*/
	TempMessageInfo MyTempInfo;
	MyTempInfo.UECode = 998;
	MyTempInfo.UEMessage = TEXT("XGTest");
	MyTempInfo.UEData = TEXT("123455");
	MyTempInfo.Sid = TEXT("11111");

	MyTempInfo.UEServerInfo.UEServerName = TEXT("XG");
	MyTempInfo.UEServerInfo.ServerVersion = TEXT("19356");

	MyTempInfo.WorkerIDs.Add(1);
	MyTempInfo.WorkerIDs.Add(3);
	MyTempInfo.WorkerIDs.Add(5);

	MyTempInfo.Coders.AddDefaulted();
	MyTempInfo.Coders.Last().Name = TEXT("XGG");
	MyTempInfo.Coders.Last().WorkYear = 3.2;

	MyTempInfo.Coders.AddDefaulted();
	MyTempInfo.Coders.Last().Name = TEXT("GGX");
	MyTempInfo.Coders.Last().WorkYear = 1.2;



	FString GoodJsonString = TEXT("");

	//注意这个模板参数会控制缩进,有些后端接受的数据可能因为网络传输的原因有空格和没空格会发生,会导致检验不通过.

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> GoodJsonWriter
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&GoodJsonString);

	GoodJsonWriter->WriteObjectStart();


	GoodJsonWriter->WriteValue(TEXT("Code"), MyTempInfo.UECode);
	GoodJsonWriter->WriteValue(TEXT("Message"), MyTempInfo.UEMessage);
	GoodJsonWriter->WriteValue(TEXT("Data"), MyTempInfo.UEData);
	GoodJsonWriter->WriteValue(TEXT("Sid"), MyTempInfo.Sid);


	{

		GoodJsonWriter->WriteObjectStart(TEXT("XG"));
		GoodJsonWriter->WriteValue(TEXT("ServerName"), MyTempInfo.UEServerInfo.UEServerName);
		GoodJsonWriter->WriteValue(TEXT("ServerVersion"), MyTempInfo.UEServerInfo.ServerVersion);
		GoodJsonWriter->WriteObjectEnd();

	}



	{
		GoodJsonWriter->WriteArrayStart(TEXT("WorkerIDs"));

		for (auto& TmpID : MyTempInfo.WorkerIDs)
		{
			GoodJsonWriter->WriteValue(TmpID);
		}
		GoodJsonWriter->WriteArrayEnd();
	}

	{
		GoodJsonWriter->WriteArrayStart(TEXT("Coders"));
		for (auto& TmpCoderInfo : MyTempInfo.Coders)
		{
			GoodJsonWriter->WriteObjectStart();

			GoodJsonWriter->WriteValue(TEXT("Name"), TmpCoderInfo.Name);
			GoodJsonWriter->WriteValue(TEXT("WorkYear"), TmpCoderInfo.WorkYear);

			GoodJsonWriter->WriteObjectEnd();
		}
		GoodJsonWriter->WriteArrayEnd();
	}



	GoodJsonWriter->WriteObjectEnd();
	GoodJsonWriter->Close();

	FString FilePath = FPaths::ProjectSavedDir() / TEXT("GoodJson.json");
	//序列化
	FFileHelper::SaveStringToFile(GoodJsonString, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);



	//反序列化
	FString BackGoodJsonString = GoodJsonString;


	TSharedPtr<FJsonObject> BackGoodJsonObject;

	TSharedRef<TJsonReader<TCHAR>>BackGoodJsonReader = TJsonReaderFactory<TCHAR>::Create(BackGoodJsonString);

	bool IsDeserialize = FJsonSerializer::Deserialize(BackGoodJsonReader, BackGoodJsonObject);
	TempMessageInfo MyBackTempInfo;
	if (IsDeserialize)
	{
		TSharedPtr<FJsonValue>  CodeJsonValue = BackGoodJsonObject->TryGetField(TEXT("Code"));

		if (CodeJsonValue.IsValid())
		{
			int32 CodeNum1 = CodeJsonValue->AsNumber();
		}
		int32 CodeNum2 = -1;
		bool bFindCoude = BackGoodJsonObject->TryGetNumberField(TEXT("Code"), CodeNum2);
		int32 CodeNum3 = BackGoodJsonObject->GetNumberField(TEXT("Code"));
		//int32 CodeNum4 = BackGoodJsonObject->GetNumberField(TEXT("Code222"));


		FString BackMessage = BackGoodJsonObject->GetStringField(TEXT("Message"));
		FString Data = BackGoodJsonObject->GetStringField(TEXT("Data"));
		FString Sid = BackGoodJsonObject->GetStringField(TEXT("Sid"));

		MyBackTempInfo.UECode = CodeNum3;
		MyBackTempInfo.UEMessage = BackMessage;
		MyBackTempInfo.UEData = Data;
		MyBackTempInfo.Sid = Sid;


		TSharedPtr<FJsonObject>  XGJsonObject = BackGoodJsonObject->GetObjectField(TEXT("XG"));

		FString BackServerName = XGJsonObject->GetStringField(TEXT("ServerName"));
		FString BackServerVersion = XGJsonObject->GetStringField(TEXT("ServerVersion"));

		MyBackTempInfo.UEServerInfo.UEServerName = BackServerName;
		MyBackTempInfo.UEServerInfo.ServerVersion = BackServerVersion;


		TArray<TSharedPtr<FJsonValue>> WorkerIDsJsonValue = BackGoodJsonObject->GetArrayField(TEXT("WorkerIDs"));

		for (auto& TmpJsonValue : WorkerIDsJsonValue)
		{
			int32 WorkerIn = TmpJsonValue->AsNumber();

			MyBackTempInfo.WorkerIDs.Add(WorkerIn);

		}


		TArray<TSharedPtr<FJsonValue>> CoderssJsonValue = BackGoodJsonObject->GetArrayField(TEXT("Coders"));

		for (auto& TmpJsonValue : CoderssJsonValue)
		{
			TSharedPtr<FJsonObject>  WorkerObject = TmpJsonValue->AsObject();

			FString Name = WorkerObject->GetStringField(TEXT("Name"));
			float WorkYear = WorkerObject->GetNumberField(TEXT("WorkYear"));

			TempCodersInfo& ThisCoder = MyBackTempInfo.Coders.AddDefaulted_GetRef();
			ThisCoder.Name = Name;
			ThisCoder.WorkYear = WorkYear;

		}




	}



}

void AXGSampleJson::GoodJson2()
{
	TempMessageInfo MyTempInfo;

	MyTempInfo.UECode = 998;
	MyTempInfo.UEMessage = TEXT("XGTest");
	MyTempInfo.UEData = TEXT("123455");
	MyTempInfo.Sid = TEXT("11111");

	MyTempInfo.UEServerInfo.UEServerName = TEXT("XG");
	MyTempInfo.UEServerInfo.ServerVersion = TEXT("19356");

	MyTempInfo.WorkerIDs.Add(1);
	MyTempInfo.WorkerIDs.Add(3);
	MyTempInfo.WorkerIDs.Add(5);

	MyTempInfo.Coders.AddDefaulted();
	MyTempInfo.Coders.Last().Name = TEXT("XGG");
	MyTempInfo.Coders.Last().WorkYear = 3.2;

	MyTempInfo.Coders.AddDefaulted();
	MyTempInfo.Coders.Last().Name = TEXT("GGX");
	MyTempInfo.Coders.Last().WorkYear = 1.2;

	FString MyJson = MyTempInfo.ToString();

	TempMessageInfo NewInfo =	TempMessageInfo::FromString(MyJson);

	NewInfo.UECode = 404;

	FString MyJson2 = NewInfo.ToString();

	int32  a =1;

}

void AXGSampleJson::BadJson()
{
	FBadMessageInfo MyBadMessageInfo;
	MyBadMessageInfo.Code = 0;
	MyBadMessageInfo.Message =TEXT("This is bad message 这是一级服务接口");
	MyBadMessageInfo.Sid =TEXT("123456");

	TempMessageInfo MyInfo;
	MyInfo.UECode = 564654;
	MyInfo.UEMessage= TEXT("二级服务接口");


	MyBadMessageInfo.Info = MyInfo.ToString();

	FString BadMessageJson;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> BadJsonWriter
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&BadMessageJson);

	BadJsonWriter->WriteObjectStart();

	BadJsonWriter->WriteValue(TEXT("Code"), MyBadMessageInfo.Code);
	BadJsonWriter->WriteValue(TEXT("Message"), MyBadMessageInfo.Message);
	BadJsonWriter->WriteValue(TEXT("Info"), MyBadMessageInfo.Info);
	BadJsonWriter->WriteValue(TEXT("Sid"), MyBadMessageInfo.Sid);


	BadJsonWriter->WriteObjectEnd();
	BadJsonWriter->Close();

	FString FilePath = FPaths::ProjectSavedDir() / TEXT("BadMessageJson.json");
	//序列化
	FFileHelper::SaveStringToFile(BadMessageJson, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
	int32 a =1;

	//反序列化
	FString BackStr = BadMessageJson;
	FBadMessageInfo BackInfo;

	TSharedPtr<FJsonObject> BackBadJsonObject;

	TSharedRef<TJsonReader<TCHAR>>BackBadJsonReader = TJsonReaderFactory<TCHAR>::Create(BackStr);

	bool IsDeserialize = FJsonSerializer::Deserialize(BackBadJsonReader, BackBadJsonObject);

	BackInfo.Code = BackBadJsonObject->GetNumberField(TEXT("Code"));
	BackInfo.Message = BackBadJsonObject->GetStringField(TEXT("Message"));
	BackInfo.Info = BackBadJsonObject->GetStringField(TEXT("Info"));
	BackInfo.Sid = BackBadJsonObject->GetStringField(TEXT("Sid"));


	TempMessageInfo MyBackInfo = TempMessageInfo::FromString(BackInfo.Info);


	int32 b = 1;

}

void AXGSampleJson::VeryBadJson()
{
	FString VeryBadJson =TEXT("");
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> VeryBadJsonWriter
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&VeryBadJson);

	VeryBadJsonWriter->WriteObjectStart();

	for (size_t i = 0; i < 4; i++)
	{
		FString PlayerName = FGuid::NewGuid().ToString();

		VeryBadJsonWriter->WriteValue(PlayerName, i);
	}

	VeryBadJsonWriter->WriteObjectEnd();
	VeryBadJsonWriter->Close();

	FString FilePath = FPaths::ProjectSavedDir() / TEXT("VeryBadJson.json");
	//序列化
	FFileHelper::SaveStringToFile(VeryBadJson, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	FString BackStr= VeryBadJson;


	TSharedPtr<FJsonObject> BackJsonObject;
	TSharedRef<TJsonReader<TCHAR>>BackJsonReader = TJsonReaderFactory<TCHAR>::Create(BackStr);
	bool IsDeserialize = FJsonSerializer::Deserialize(BackJsonReader, BackJsonObject);

	TMap<FString,float> PlayerInfos;
	//TArray<FPlayerInfo>;

	for (auto&Tmp  : BackJsonObject->Values)
	{
		PlayerInfos.Add(Tmp.Key, Tmp.Value->AsNumber());
	}




	int32 ab =1;


}

void AXGSampleJson::GoodSturctJson()
{

	FXGSampleServerInfo ServerInfo;

	ServerInfo.ServerName = TEXT("XGServer");

	ServerInfo.ServerVersion = TEXT("1.3.3");

	ServerInfo.NotWrite = TEXT("没有被序列化");


	FString ServerInfoJson;
	//序列化
	FJsonObjectConverter::UStructToJsonObjectString(ServerInfo, ServerInfoJson);

	//反序列化
	FString BackServerInfoJson = ServerInfoJson;

	FXGSampleServerInfo BackServerInfo;

	FJsonObjectConverter::JsonObjectStringToUStruct(BackServerInfoJson, &BackServerInfo);


	int32 a =1;


}

void AXGSampleJson::GoodSturctJson2()
{

	FXGSampleMessageInfo MessageInfo;

	MessageInfo.Code = 10087;
	MessageInfo.Message =TEXT("这是测试信息");
	MessageInfo.Data =TEXT("1111111");
	MessageInfo.Sid = TEXT("753415");

	MessageInfo.Info.ServerName = TEXT("XGServer");
	MessageInfo.Info.ServerVersion = TEXT("1.3.4");

	MessageInfo.WorkerIDs.Add(2);
	MessageInfo.WorkerIDs.Add(4);
	MessageInfo.WorkerIDs.Add(6);


	MessageInfo.Coders.AddDefaulted();
	MessageInfo.Coders.Last().Name = TEXT("XG");
	MessageInfo.Coders.Last().WorkYear = 3.2;

	MessageInfo.Coders.AddDefaulted();
	MessageInfo.Coders.Last().Name = TEXT("GX");
	MessageInfo.Coders.Last().WorkYear = 1.3;

	MessageInfo.TempTime = FDateTime::Now();

	FString MessageInfoJson;
	//序列化
	FJsonObjectConverter::UStructToJsonObjectString(MessageInfo, MessageInfoJson);

	//反序列化
	FString BackMessageInfoJson = MessageInfoJson;

	FXGSampleMessageInfo BackMessageInfo;

	FJsonObjectConverter::JsonObjectStringToUStruct(BackMessageInfoJson, &BackMessageInfo);

	int32 a = 1;




}

// Called when the game starts or when spawned
void AXGSampleJson::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AXGSampleJson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString TempMessageInfo::ToString()
{
	FString GoodJsonString = TEXT("");

	//注意这个模板参数会控制缩进,有些后端接受的数据可能因为网络传输的原因有空格和没空格会发生,会导致检验不通过.

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> GoodJsonWriter
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&GoodJsonString);

	GoodJsonWriter->WriteObjectStart();


	GoodJsonWriter->WriteValue(TEXT("Code"), UECode);
	GoodJsonWriter->WriteValue(TEXT("Message"), UEMessage);
	GoodJsonWriter->WriteValue(TEXT("Data"), UEData);
	GoodJsonWriter->WriteValue(TEXT("Sid"), Sid);


	{

		GoodJsonWriter->WriteObjectStart(TEXT("XG"));
		GoodJsonWriter->WriteValue(TEXT("ServerName"), UEServerInfo.UEServerName);
		GoodJsonWriter->WriteValue(TEXT("ServerVersion"), UEServerInfo.ServerVersion);
		GoodJsonWriter->WriteObjectEnd();

	}



	{
		GoodJsonWriter->WriteArrayStart(TEXT("WorkerIDs"));

		for (auto& TmpID : WorkerIDs)
		{
			GoodJsonWriter->WriteValue(TmpID);
		}
		GoodJsonWriter->WriteArrayEnd();
	}

	{
		GoodJsonWriter->WriteArrayStart(TEXT("Coders"));
		for (auto& TmpCoderInfo : Coders)
		{
			GoodJsonWriter->WriteObjectStart();

			GoodJsonWriter->WriteValue(TEXT("Name"), TmpCoderInfo.Name);
			GoodJsonWriter->WriteValue(TEXT("WorkYear"), TmpCoderInfo.WorkYear);

			GoodJsonWriter->WriteObjectEnd();
		}
		GoodJsonWriter->WriteArrayEnd();
	}



	GoodJsonWriter->WriteObjectEnd();
	GoodJsonWriter->Close();
	return GoodJsonString;
}

TempMessageInfo TempMessageInfo::FromString(const FString& InJsonString)
{


	//反序列化
	FString BackGoodJsonString = InJsonString;


	TSharedPtr<FJsonObject> BackGoodJsonObject;

	TSharedRef<TJsonReader<TCHAR>>BackGoodJsonReader = TJsonReaderFactory<TCHAR>::Create(BackGoodJsonString);

	bool IsDeserialize = FJsonSerializer::Deserialize(BackGoodJsonReader, BackGoodJsonObject);
	TempMessageInfo MyBackTempInfo;
	if (IsDeserialize)
	{
		TSharedPtr<FJsonValue>  CodeJsonValue = BackGoodJsonObject->TryGetField(TEXT("Code"));

		if (CodeJsonValue.IsValid())
		{
			int32 CodeNum1 = CodeJsonValue->AsNumber();
		}
		int32 CodeNum2 = -1;
		bool bFindCoude = BackGoodJsonObject->TryGetNumberField(TEXT("Code"), CodeNum2);
		int32 CodeNum3 = BackGoodJsonObject->GetNumberField(TEXT("Code"));
		//int32 CodeNum4 = BackGoodJsonObject->GetNumberField(TEXT("Code222"));

		MyBackTempInfo.UECode = CodeNum3;
		MyBackTempInfo.UEMessage = BackGoodJsonObject->GetStringField(TEXT("Message"));
		MyBackTempInfo.UEData = BackGoodJsonObject->GetStringField(TEXT("Data"));
		MyBackTempInfo.Sid = BackGoodJsonObject->GetStringField(TEXT("Sid"));

	
		TSharedPtr<FJsonObject>  XGJsonObject = BackGoodJsonObject->GetObjectField(TEXT("XG"));

		FString BackServerName = XGJsonObject->GetStringField(TEXT("ServerName"));
		FString BackServerVersion = XGJsonObject->GetStringField(TEXT("ServerVersion"));

		MyBackTempInfo.UEServerInfo.UEServerName = BackServerName;
		MyBackTempInfo.UEServerInfo.ServerVersion = BackServerVersion;


		TArray<TSharedPtr<FJsonValue>> WorkerIDsJsonValue = BackGoodJsonObject->GetArrayField(TEXT("WorkerIDs"));

		for (auto& TmpJsonValue : WorkerIDsJsonValue)
		{
			int32 WorkerIn = TmpJsonValue->AsNumber();

			MyBackTempInfo.WorkerIDs.Add(WorkerIn);

		}


		TArray<TSharedPtr<FJsonValue>> CoderssJsonValue = BackGoodJsonObject->GetArrayField(TEXT("Coders"));

		for (auto& TmpJsonValue : CoderssJsonValue)
		{
			TSharedPtr<FJsonObject>  WorkerObject = TmpJsonValue->AsObject();

			FString Name = WorkerObject->GetStringField(TEXT("Name"));
			float WorkYear = WorkerObject->GetNumberField(TEXT("WorkYear"));

			TempCodersInfo& ThisCoder = MyBackTempInfo.Coders.AddDefaulted_GetRef();
			ThisCoder.Name = Name;
			ThisCoder.WorkYear = WorkYear;

		}




	}


	return MyBackTempInfo;
}
