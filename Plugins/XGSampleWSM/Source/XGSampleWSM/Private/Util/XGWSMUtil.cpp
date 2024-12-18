#include "XGWSMUtil.h"

#include "JsonObjectConverter.h"
#include "Containers/ArrayView.h"
#include "Containers/StringConv.h"
#include "Misc/Base64.h"

TArray<uint8> UXGWSMUtilBPLibrary::XGMessageEncode(const FString& InMessage)
{
	FTCHARToUTF8 UTF8Str(InMessage);

	FString  Base64Str = FBase64::Encode((uint8*)UTF8Str.Get(), UTF8Str.Length());
	TArray<uint8> UTF8Array;
	const TCHAR* CharArray = *Base64Str;
	while (*CharArray != '\0')
	{
		uint8 ByteValue = static_cast<uint8>(*CharArray);
		UTF8Array.Add(ByteValue);

		CharArray++;
	}
	return UTF8Array;

}

FString UXGWSMUtilBPLibrary::XGMessageDecode(TArray<uint8>& InData)
{

	int32 Index = 0;
	FString Base64Str = TEXT("");
	while (Index < InData.Num())
	{
		char ByteValue = static_cast<char>(InData[Index]);
		Base64Str += ByteValue;

		Index++;
	}

	TArray<uint8> UTF8Array;

	FBase64::Decode(Base64Str, UTF8Array);

	FUTF8ToTCHAR Converter(reinterpret_cast<ANSICHAR*>(UTF8Array.GetData()), UTF8Array.Num());

	FString OutMessageString(Converter.Length(), Converter.Get());

	return OutMessageString;

}

FString UXGWSMUtilBPLibrary::XGMessageDecode(const uint8* InDataPtr, int32 InDataSize)
{

	int32 Index = 0;
	FString Base64Str = TEXT("");
	while (Index < InDataSize)
	{
		char ByteValue = static_cast<char>(InDataPtr[Index]);
		Base64Str += ByteValue;

		Index++;
	}

	TArray<uint8> UTF8Array;

	FBase64::Decode(Base64Str, UTF8Array);

	FUTF8ToTCHAR Converter(reinterpret_cast<ANSICHAR*>(UTF8Array.GetData()), UTF8Array.Num());

	FString OutMessageString(Converter.Length(), Converter.Get());

	return OutMessageString;
}
