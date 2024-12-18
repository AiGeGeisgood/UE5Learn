#include "XGSampleXFLinkBase.h"

//#include "EncryptionContextOpenSSL.h"
#include "Misc/Base64.h"


THIRD_PARTY_INCLUDES_START
#include <openssl/bn.h>
#include <openssl/obj_mac.h>
#include <openssl/opensslv.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/err.h>
THIRD_PARTY_INCLUDES_END


#if WITH_SSL
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/opensslconf.h>
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>

#endif


FXGSampleXFLnikBaseSHA256 FXGSampleXFLinkBase::HmacSha256(const uint8* Input, size_t InputLen, const uint8* Key, size_t KeyLen)
{

	FXGSampleXFLnikBaseSHA256 Output;

	unsigned int OutputLen = 0;

	HMAC(EVP_sha256(), Key, KeyLen, (const unsigned char*)Input, InputLen, Output.Digest, &OutputLen);

	return Output;
}

FString FXGSampleXFLinkBase::BDHMACSHA256(const FString& InAPPSecreet, const FString& InData)
{
	int len1 = strlen((char*)TCHAR_TO_UTF8(*InAPPSecreet));
	int len2 = strlen((char*)TCHAR_TO_UTF8(*InData));

	FXGSampleXFLnikBaseSHA256 BaseSHA256 = HmacSha256((uint8_t*)TCHAR_TO_UTF8(*InData), len2, (uint8_t*)TCHAR_TO_UTF8(*InAPPSecreet), len1);

	FString HexStr = FString::FromHexBlob(BaseSHA256.Digest, 32);

	HexStr.ToLowerInline();

	return HexStr;
}

FString FXGSampleXFLinkBase::XGHMACSHA256(const FString& InAPPSecreet, const FString& InData)
{
	FTCHARToUTF8  AppSecretData(InAPPSecreet);
	FTCHARToUTF8  Data(InData);

	FXGSampleXFLnikBaseSHA256 XunFeiBaseSHA256 = FXGSampleXFLinkBase::HmacSha256
	((uint8_t*)Data.Get(), Data.Length(), (uint8_t*)AppSecretData.Get(), AppSecretData.Length());

	FString RetStr = TEXT("");

	RetStr = FBase64::Encode(XunFeiBaseSHA256.Digest, 32);

	return RetStr;
}


void FXGSampleXFLinkBase::AssembleAuthUrl(FString IniFlyTekURL, FString InAPISecret, FString InAPIKey, FString& OutAuthURL, FString& OutProtocol, bool bGet)
{

	TArray<FString> URLParts;

	IniFlyTekURL.ParseIntoArray(URLParts, TEXT("/"), true);

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

	FString host = TEXT("host: ") + URLDomain + TEXT("\n");

	FString HttpDate = FDateTime::Now().UtcNow().ToHttpDate();

	FString date = TEXT("date: ") + HttpDate + TEXT("\n");

	FString requireLine = TEXT("");

	if (bGet)
	{
		requireLine = TEXT("GET ") + URLPath + TEXT(" HTTP/1.1");
	}
	else 
	{
		requireLine = TEXT("POST ") + URLPath + TEXT(" HTTP/1.1");
	}



	FString signature_origin = host + date + requireLine;

	FString	signature = XGHMACSHA256(InAPISecret, signature_origin);

	FString authorization_origin =
		FString::Printf(TEXT("api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""),
		*InAPIKey, *signature);

	FString authorization = FBase64::Encode(authorization_origin);

	OutProtocol = URLParts[0].Replace(TEXT(":"), TEXT(""));

	OutAuthURL = IniFlyTekURL+ FString::Printf(TEXT("?authorization=%s&date=%s&host=%s"), *authorization, *URLEncode(HttpDate), *URLDomain);


}

FString FXGSampleXFLinkBase::URLEncode(FString InURL)
{
	InURL.ReplaceInline(TEXT("%"), TEXT("%25"));
	InURL.ReplaceInline(TEXT("+"), TEXT("%2B"));
	InURL.ReplaceInline(TEXT(" "), TEXT("%20"));
	InURL.ReplaceInline(TEXT("/"), TEXT("%2F"));
	InURL.ReplaceInline(TEXT("?"), TEXT("%3F"));
	InURL.ReplaceInline(TEXT("#"), TEXT("%23"));
	InURL.ReplaceInline(TEXT("&"), TEXT("%26"));
	InURL.ReplaceInline(TEXT("="), TEXT("%3D"));
	InURL.ReplaceInline(TEXT(":"), TEXT("%3A"));
	InURL.ReplaceInline(TEXT(","), TEXT("%2C"));

	return InURL;
}

FString FXGSampleXFLinkBase::Base64UTF8StringToTCharString(const FString& InBase64UTF8String)
{
	FString TextString = TEXT("");

	TArray<uint8> TextData;

	FBase64::Decode(InBase64UTF8String, TextData);


	TextString = FUTF8ToTCHAR(reinterpret_cast<const UTF8CHAR*>(TextData.GetData()), TextData.Num());





	return TextString;
}

TMap<FString, FString> FXGSampleXFLinkBase::GenerateBDHeaders(const FString& InURL, const FString& InAPIKey, const FString& InInAPISecret)
{
	TMap<FString, FString> OutHeaders;

	OutHeaders.Add(TEXT("Content-Type"), TEXT("application/json"));

	FString Timestamp = FDateTime::UtcNow().ToIso8601();

	int32 IndexPoint = Timestamp.Find(TEXT("."));

	Timestamp = Timestamp.Left(IndexPoint) + TEXT("Z");

	//Timestamp= TEXT("2024-04-16T02:33:31Z");

	OutHeaders.Add(TEXT("x-bce-date"), Timestamp);

	OutHeaders.Add(TEXT("Authorization"), GenerateBDAuthorizationToken(InURL, InAPIKey, InInAPISecret, Timestamp));



	return OutHeaders;
}

FString FXGSampleXFLinkBase::GenerateBDAuthorizationToken(const FString& InURL, const FString& InAPIKey, const FString& InInAPISecret, const FString& InTimestamp, int32 ValiditySecond)
{

	FString Host = TEXT("");
	FString Point = TEXT("");

	AnalyseBDURL(InURL, Host, Point);

	FString authStringPrefix = TEXT("bce-auth-v1/") + InAPIKey + TEXT("/") + InTimestamp + TEXT("/") + FString::FromInt(ValiditySecond);

	FString canonicalRequest = TEXT("");

	canonicalRequest += TEXT("POST");
	canonicalRequest += TEXT("\n");
	canonicalRequest += Point;
	canonicalRequest += TEXT("\n");
	canonicalRequest += TEXT("\n");
	canonicalRequest += TEXT("host:") + Host;
	canonicalRequest += TEXT("\n");
	canonicalRequest += TEXT("x-bce-date:") + URLEncode(InTimestamp);

	FString signingKey = BDHMACSHA256(InInAPISecret, authStringPrefix);

	FString signature = BDHMACSHA256(signingKey, canonicalRequest);

	FString Authorization = TEXT("");

	Authorization += authStringPrefix + TEXT("/host;x-bce-date/") + signature;

	return Authorization;
}

void FXGSampleXFLinkBase::AnalyseBDURL(const FString& InURL, FString& OutHost, FString& OutPoint)
{

	TArray<FString> URLParts;

	InURL.ParseIntoArray(URLParts, TEXT("/"), true);

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
	OutHost = URLDomain;
	OutPoint = URLPath;
}
