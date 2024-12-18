#include "XGStringActor.h"
#include "Kismet/KismetStringLibrary.h"

#define LOCTEXT_NAMESPACE "XGStringActor"
// Sets default values
AXGStringActor::AXGStringActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGStringActor::InitString()
{
	FString TestString = TEXT("This is a str test");

	//FStrirng->FName
	FName TestName = FName(*TestString);

	//FString->FText
	FText TestText = FText::FromString(TestString);

	//FName->FString
	FString BackString = TestName.ToString();

	//FText->FString
	FString BackString2 = TestText.ToString();

	//FText->FName 不能直接转换 需要过渡
	FName BackName = FName(TestText.ToString());

	//FName->FText
	FText BackText = FText::FromName(TestName);

	int32 a = 1;

}

void AXGStringActor::PropertyString()
{
	//float ->FString
	float MyFloat = 109.3f;
	FString FloatFString = FString::SanitizeFloat(MyFloat);

	//int32->FString
	int32 MyInt = 99;
	FString IntString = FString::FromInt(MyInt);

	//bool->FString
	bool bMyBool = true;
	FString BoolString = bMyBool ? TEXT("true") : TEXT("false");

	//FVector->FString
	FVector MyLocation = FVector(100.0f, 20.f, 30.f);
	FString MyVectorString = MyLocation.ToString();

	//FVector2D->FString
	FVector2D MyVector2D = FVector2D(100.0f, 20.f);
	FString MyVector2DString = MyVector2D.ToString();

	//FRotator->FString
	FRotator MyRotator = FRotator(40.f, 50.f, 60.f);
	FString MyRotatorString = MyRotator.ToString();

	//FLinearColor->FString
	FLinearColor MyColor = FLinearColor::Blue;
	FString MyColorString = MyColor.ToString();

	//UObject->FString
	AActor* MyActor = this;
	FString MyActorString = (MyActor != nullptr) ? MyActor->GetName() : FString(TEXT("None"));

	//FString->bool
	bool MyBackBool = BoolString.ToBool();

	//FString->int
	int32 MyBackint = FCString::Atoi(*IntString);

	//FString->float
	float MyFloatBool = FCString::Atof(*FloatFString);

	int32 a = 1;

}

void AXGStringActor::ModifyString()
{
	FString Str1 = TEXT("Test");
	FString Str2 = TEXT("test");

	bool bEqual = Str1 == Str2;

	bool bEqualCase = Str1.Equals(Str2, ESearchCase::CaseSensitive);

	FString Str = TEXT("UnrealEngine5");

	bool bContainStart = Str.Contains(TEXT("Engine"), ESearchCase::IgnoreCase, ESearchDir::FromStart);
	bool bContainEnd = Str.Contains(TEXT("Engine"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	int32 StartIndex = Str.Find(TEXT("real"), ESearchCase::IgnoreCase, ESearchDir::FromStart);
	int32 EndIndex = Str.Find(TEXT("real"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);


	int32 a = 11;
}

void AXGStringActor::ModifyString2()
{
	FString Str = TEXT("This is my good code !");

	Str += TEXT("What is your code?");

	FString NetString = Str + TEXT("Today is a good day");

	TCHAR MYChar = *TEXT("A");
	NetString.AppendChar(MYChar);

	NetString.Append(TEXT("BCD"));

	FString AnotherNetString = NetString.Replace(TEXT("BCD"), TEXT("BCDEF"));

	int32 ReplaceNum = AnotherNetString.ReplaceInline(TEXT("Today"), TEXT("Tomorrow"));

	int32 a = 11;

}

void AXGStringActor::LogString()
{
	float  TimeSeconds = 1802.456f;
	// only minutes and seconds are relevant
	const int32 TotalSeconds = FMath::Max(0, FMath::TruncToInt(TimeSeconds) % 3600);
	const int32 NumMinutes = TotalSeconds / 60;
	const int32 NumSeconds = TotalSeconds % 60;

	const FString TimeDesc = FString::Printf(TEXT("%02d:%02d"), NumMinutes, NumSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Data:[%.2f]==>%02d:%02d"), TimeSeconds, NumMinutes, NumSeconds);

}

void AXGStringActor::OperateString()
{
	FString MyStr = TEXT("This is my good code !");

	FString LeftStr = MyStr.Left(5);

	FString MidStr = MyStr.Mid(5);

	FString RightStr = MyStr.Right(6);

	int32 aa = 1;




}

void AXGStringActor::OperateString2()
{
	FString MyStr = TEXT("MyLife=998");

	FString MyLeftKey = TEXT("");
	FString MyRightKey = TEXT("");

	MyStr.Split(TEXT("="), &MyLeftKey, &MyRightKey);

	FString MyArrayStr = TEXT("MyLife,MyMoney,MyHealth,MyMana");

	TArray<FString> MyStrArray;
	MyArrayStr.ParseIntoArray(MyStrArray, TEXT(","));

	FString UpperStr = MyStrArray[0].ToUpper();
	FString LowerStr = MyStrArray[1].ToLower();

	int32 a = 1;

}

void AXGStringActor::OperateURL()
{

	FString iFlyTekURL = TEXT("ws://spark-api.xf-yun.com/v4.0/chat");

	TArray<FString> URLParts;

	iFlyTekURL.ParseIntoArray(URLParts, TEXT("/"), true);

	FString URLDomain = URLParts[1];

	FString URLPath = TEXT("");

	for (int32 Index = 0; Index < URLParts.Num(); Index++)
	{
		if (Index > 1)
		{
			URLPath += TEXT("/");
			URLPath += URLParts[Index];

		}

	}

	int32 a = 1;




}

void AXGStringActor::TransformString()
{
	FString Str = TEXT("This is my good code !");
	char* MyAnsiChar = TCHAR_TO_ANSI(*Str);
	FString MyBackStr = ANSI_TO_TCHAR(MyAnsiChar);



	FString StrUTF8 = TEXT("This is my good code !生命是多么美好!");
	char* MyAnsiCharUTF8 = TCHAR_TO_UTF8(*StrUTF8);
	FString MyBackStrUTF8 = UTF8_TO_TCHAR(MyAnsiCharUTF8);


	FString StrLong = TEXT("This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !");
	char* MyAnsiCharLong = TCHAR_TO_ANSI(*StrLong);
	FString MyBackStrLong = ANSI_TO_TCHAR(MyAnsiCharLong);

	WrongUse(MyAnsiCharUTF8);

	int32 a = 1;

}

void AXGStringActor::WrongUse(char* InStr)
{
	//!!!! 错误的!!

}

void AXGStringActor::TransformStringRight()
{
	FString StrLong = TEXT("This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !This is my good code !美好的生活ask来得及喀拉喀什角动量喀什角动量卡就是");

	FTCHARToUTF8 Convert(*StrLong);

	TArray<uint8> Data;

	Data.Append((uint8*)Convert.Get(), Convert.Length());

	FUTF8ToTCHAR BackConvert((const ANSICHAR*)Data.GetData(), Data.Num());

	FString UTF8Text(BackConvert.Length(), BackConvert.Get());


	auto UTF8String = StringCast<UTF8CHAR>(*StrLong);

	TArray<uint8> NewMethodData;
	NewMethodData.SetNum(UTF8String.Length());
	NewMethodData.Add(0);

	FMemory::Memcpy(NewMethodData.GetData(), UTF8String.Get(), UTF8String.Length());

	auto Cnv = StringCast<TCHAR>((const UTF8CHAR*)NewMethodData.GetData(), NewMethodData.Num());

	FString FinalStr(Cnv.Get(), Cnv.Length());

	int32 a = 1;

}

void AXGStringActor::FNameTest()
{
	FName MyFName1 = FName(TEXT("OnlyTest"));
	FName MyFName2 = FName(TEXT("NotTest"));

	bool bEqual = MyFName1== MyFName2;

	int32 CompareResult = MyFName1.Compare(MyFName2);

	if (FName(TEXT("pelvis"), FNAME_Find) != NAME_None)
	{
		int32 a =1;
		// Do something

	}

	if (FName(TEXT("OnlyTest"), FNAME_Find) != NAME_None)
	{
		int32 bb = 1;
		// Do something

	}

	if (FName(TEXT("OnlyTest22"), FNAME_Find) != NAME_None)
	{
		int32 bb = 1;
		// Do something

	}


	int32 aaa = 1;
}

FText AXGStringActor::FTextTest()
{
	FText MyCodeText = LOCTEXT("OwnCode", "MyCode");

	FText MyCodeTypeText  = NSLOCTEXT("CodeTypeNamespace", "CodeType", "XiaoGangCode");

	return MyCodeText;


}

FText AXGStringActor::FTextFormat()
{
	FString PlayerName =TEXT("XG");

	FText PlayerText = FText::Format(LOCTEXT("PlayerNameFmt", "{0} is really cool"), FText::FromString(PlayerName));

	FText NumText=  FText::AsNumber(15689.33f);

	FText PercentText = FText::AsPercent(0.33f);

	FText MemoryText = FText::AsMemory(1234);

	FText MoneyText= FText::AsCurrencyBase(650, TEXT("EUR"));

	FDateTime MyDateTime=  FDateTime::Now();

	FDateTime::UtcNow();

	FString  CurrentLocalTime =MyDateTime.ToHttpDate();

	FText DateText = FText::AsDate(MyDateTime);

	FText TimeText = FText::AsTime(MyDateTime);



	return FText();
}

// Called when the game starts or when spawned
void AXGStringActor::BeginPlay()
{
	Super::BeginPlay();

	FString TestString = FString(TEXT("This is a test"));

	FString TestString2 = TEXT("This is a test");

	FString TestString32 = "This is a test";





}

// Called every frame
void AXGStringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#undef LOCTEXT_NAMESPACE