#pragma once
#include "CoreMinimal.h"


/**
 * Save the result after HmacSha256 encryption
 * 
 */
struct XGSAMPLEXFLINKLIBRARY_API FXGSampleXFLnikBaseSHA256
{
	uint8 Digest[32];

};
/**
 * 
 * Exposed  to use encryption method
 * 
 * 
 */
class XGSAMPLEXFLINKLIBRARY_API FXGSampleXFLinkBase
{

public:
	/**
	 * HmacSha256
	 *
	 * @param Input		Input What you want to encrypt data ptr.
	 * @param InputLen	The lenth of data.
	 * @param Key		The Key ptr  which you want to encrypt data with.
	 * @param KeyLen	The lenth of the key.
	 * @return FXGSampleXFLnikBaseSHA256	Encryption Resulte
	 */
	static FXGSampleXFLnikBaseSHA256 HmacSha256(const uint8* Input, size_t InputLen, const uint8* Key, size_t KeyLen);


	static FString BDHMACSHA256(const FString& InAPPSecreet, const FString& InData);


	static	FString XGHMACSHA256(const FString& InAPPSecreet, const FString& InData);


	static void AssembleAuthUrl(FString IniFlyTekURL, FString InAPISecret, FString InAPIKey, FString& OutAuthURL, FString& OutProtocol,bool bGet=false);


	static FString URLEncode(FString InURL);

	//把一段经过Base64j加密后的UTF8字符串解密成虚幻引擎字符串
	static FString Base64UTF8StringToTCharString(const FString& InBase64UTF8String);


	static TMap<FString, FString> GenerateBDHeaders(const FString& InURL, const FString& InAPIKey, const FString& InInAPISecret);

	static FString GenerateBDAuthorizationToken(const FString& InURL, const FString& InAPIKey,
	const FString& InInAPISecret, 
	const FString& InTimestamp, int32 ValiditySecond = 1800);

	static void AnalyseBDURL(const FString& InURL, FString& OutHost, FString& OutPoint);



};