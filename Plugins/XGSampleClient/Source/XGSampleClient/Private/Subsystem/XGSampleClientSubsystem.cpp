#include "XGSampleClientSubsystem.h"
#include "JsonObjectConverter.h"
#include "Misc/SecureHash.h"

#include "XGSampleClientSettings.h"
#include "LogXGSampleClient.h"

#include "XGSampleServerResponseType.h"
void UXGSampleClientSubsystem::CheckVersion(FXGCheckVersionDelegate CheckVersionDelegate)
{
	EXGSampleServerResult ResponseResult;

	FString DelegateGuid = FGuid::NewGuid().ToString();

	//1.获取本地配置的远端服务器信息
	if (!LoadXGLoginServerIPAndPort())
	{
		ResponseResult = EXGSampleServerResult::ParseServerIpAndPortFailed;
		CheckVersionDelegate.ExecuteIfBound(EXGSampleRequestType::RegisterReq, DelegateGuid, false, ResponseResult);
		return;
	}

	//2.本地记录代理,等待服务端回调
	CheckVersionDelegatesMap.Add(DelegateGuid, CheckVersionDelegate);

	//3.收集版本信息
	FString RequestData = TEXT("");
	FXGCheckVersionReqData CheckVersionReqData;
	CheckVersionReqData.ClientVersion = UXGSampleClientSettings::GetXGLoginServerSettings()->ClientVersion;
	FJsonObjectConverter::UStructToJsonObjectString(CheckVersionReqData, RequestData);



	//4.发出版本校验申请请求
	SendHttp(EXGSampleRequestType::CheckVersionReq, RequestData, DelegateGuid);



}

void UXGSampleClientSubsystem::Register(FXGRegisterReqData& UserRegisterData, FXGRegisterDelegate RegisterDelegate)
{
	//1.校验数据正确与否
	EXGSampleServerResult ResponseResult;

	FString DelegateGuid = FGuid::NewGuid().ToString();

	bool bRightData = UserRegisterData.IsValid(ResponseResult);

	if (!bRightData)
	{
		FXGRegisterRespData RespData;
		RegisterDelegate.ExecuteIfBound(EXGSampleRequestType::RegisterReq, DelegateGuid, false, ResponseResult, RespData);

		return;
	}

	//2.获取本地配置的远端服务器信息
	if (!LoadXGLoginServerIPAndPort())
	{
		ResponseResult = EXGSampleServerResult::ParseServerIpAndPortFailed;
		FXGRegisterRespData RespData;
		RegisterDelegate.ExecuteIfBound(EXGSampleRequestType::RegisterReq, DelegateGuid, false, ResponseResult, RespData);
		return;
	}

	//3.本地记录代理,等待服务端回调
	RegisterDelegates.Add(DelegateGuid, RegisterDelegate);


	//4.反序列化数据
	UserRegisterData.Password = MD5HashPassword(UserRegisterData.Password);

	FString ReqDataStr = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(UserRegisterData, ReqDataStr);

	//5.发出登录请求
	SendHttp(EXGSampleRequestType::RegisterReq, ReqDataStr, DelegateGuid);

}

void UXGSampleClientSubsystem::Login(FXGLoginReqData& LoginReqData, FXGLoginDelegate LoginDelegate)
{
	//1.校验数据正确与否
	EXGSampleServerResult ResponseResult;

	FString DelegateGuid = FGuid::NewGuid().ToString();

	bool bRightLoginData = LoginReqData.IsValid(ResponseResult);

	if (!bRightLoginData)
	{
		FXGLoginRespData LoginRespData;
		LoginDelegate.ExecuteIfBound(EXGSampleRequestType::LoginReq, DelegateGuid, false, ResponseResult, LoginRespData);

		return;
	}

	//2.获取本地配置的远端服务器信息
	if (!LoadXGLoginServerIPAndPort())
	{
		ResponseResult = EXGSampleServerResult::ParseServerIpAndPortFailed;
		FXGLoginRespData LoginRespData;
		LoginDelegate.ExecuteIfBound(EXGSampleRequestType::LoginReq, DelegateGuid, false, ResponseResult, LoginRespData);
		return;
	}

	//3.本地记录代理,等待服务端回调
	LoginDelegates.Add(DelegateGuid, LoginDelegate);

	//4.反序列化数据
	LoginReqData.Password = MD5HashPassword(LoginReqData.Password);

	FString LoginReqDataStr = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(LoginReqData, LoginReqDataStr);

	//5.发出登录请求
	SendHttp(EXGSampleRequestType::LoginReq, LoginReqDataStr, DelegateGuid);


}

void UXGSampleClientSubsystem::SendHttp(EXGSampleRequestType InRequestType, const FString& InRequestDataStr, const FString& InDelegateGuid)
{

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FXGSampleServerRequest XGSampleServerRequest;
	XGSampleServerRequest.RequestType = InRequestType;
	XGSampleServerRequest.Data = InRequestDataStr;

	FString RequestBody;
	FJsonObjectConverter::UStructToJsonObjectString(XGSampleServerRequest, RequestBody);


	Request->OnProcessRequestComplete().BindUObject(this, &UXGSampleClientSubsystem::OnResponseReceived, InRequestType, InDelegateGuid);

	FString URL = FString::Printf(TEXT("http://%s:%s/Login"), *XGLoginServerIP, *XGLoginServerPort);

	Request->SetURL(URL);

	Request->SetVerb("POST");

	Request->SetHeader("Content-Type", "application/json");

	Request->SetHeader("XGLoginServerToken", TCHAR_TO_ANSI(*GenerateToken()));

	Request->SetContentAsString(RequestBody);

	Request->ProcessRequest();

}

void UXGSampleClientSubsystem::OnResponseReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, EXGSampleRequestType RequestType, FString DelegateGuid)
{

	EXGSampleServerResult ServerResult = EXGSampleServerResult::NetError;

	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{
		UE_LOG(LogXGSampleClient, Display, TEXT("[%s]"), *FString(__FUNCTION__));

		FString ResponseJson = HttpResponse->GetContentAsString();

		UE_LOG(LogXGSampleClient, Display, TEXT("[%s]"), *ResponseJson);

		FXGSampleServerResponse  ServerResponse;

		if (FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &ServerResponse))
		{
			if (ServerResponse.ResultType == EXGSampleServerResult::Succeeded)
			{

				ProcessHttpResponse(ServerResponse, RequestType, DelegateGuid);

				return;


			}
			else
			{
				ServerResult = ServerResponse.ResultType;
			}



		}
		else
		{

			ServerResult = EXGSampleServerResult::ParseServerResponseFailed;

		}



	}

	switch (RequestType)
	{
	case EXGSampleRequestType::CheckVersionReq:

		if (CheckVersionDelegatesMap.Num() > 0)
		{
			if (FXGCheckVersionDelegate* CheckVersionDelegate = CheckVersionDelegatesMap.Find(DelegateGuid))
			{
				CheckVersionDelegate->ExecuteIfBound(RequestType, DelegateGuid, false, ServerResult);
				CheckVersionDelegatesMap.Remove(DelegateGuid);
			}
		}


		break;
	case EXGSampleRequestType::RegisterReq:
		break;
	case EXGSampleRequestType::LoginReq:

		if (LoginDelegates.Num() > 0)
		{
			if (FXGLoginDelegate* LoginDelegate = LoginDelegates.Find(DelegateGuid))
			{
				FXGLoginRespData LoginRespData;
				LoginDelegate->ExecuteIfBound(RequestType, DelegateGuid,false, ServerResult, LoginRespData);
				LoginDelegates.Remove(DelegateGuid);
			}
		}


		break;
	case EXGSampleRequestType::MAX:
		break;
	default:
		break;
	}




}

void UXGSampleClientSubsystem::ProcessHttpResponse(const FXGSampleServerResponse& LoginServerResponse, EXGSampleRequestType RequestType, const FString& DelegateGuid)
{

	switch (LoginServerResponse.ResponseType)
	{
	case EXGLoginServerResponseType::CheckVersionResp:

		if (CheckVersionDelegatesMap.Num() > 0)
		{
			if (FXGCheckVersionDelegate* CheckVersionDelegate = CheckVersionDelegatesMap.Find(DelegateGuid))
			{
				CheckVersionDelegate->ExecuteIfBound(RequestType, DelegateGuid, LoginServerResponse.bResult, LoginServerResponse.ResultType);
				CheckVersionDelegatesMap.Remove(DelegateGuid);
			}
		}

		break;
	case EXGLoginServerResponseType::RegisterResp:

		if (RegisterDelegates.Num() > 0)
		{
			if (FXGRegisterDelegate* RegisterDelegate = RegisterDelegates.Find(DelegateGuid))
			{
				FXGRegisterRespData RespData;

				if (FJsonObjectConverter::JsonObjectStringToUStruct(LoginServerResponse.Data, &RespData))
				{
					RegisterDelegate->ExecuteIfBound(RequestType, DelegateGuid, LoginServerResponse.bResult, LoginServerResponse.ResultType, RespData);
				}
				else
				{
					RegisterDelegate->ExecuteIfBound(RequestType, DelegateGuid, false, EXGSampleServerResult::ParseServerResponseDataFailed, RespData);

				}
				RegisterDelegates.Remove(DelegateGuid);


			}
		}

		break;
	case EXGLoginServerResponseType::LoginResp:

		if (LoginDelegates.Num() > 0)
		{
			if (FXGLoginDelegate* LoginDelegate = LoginDelegates.Find(DelegateGuid))
			{
				FXGLoginRespData LoginRespData;

				if (FJsonObjectConverter::JsonObjectStringToUStruct(LoginServerResponse.Data, &LoginRespData))
				{
					LoginDelegate->ExecuteIfBound(RequestType, DelegateGuid, LoginServerResponse.bResult, LoginServerResponse.ResultType, LoginRespData);
				}
				else
				{
					LoginDelegate->ExecuteIfBound(RequestType, DelegateGuid, false, EXGSampleServerResult::ParseServerResponseDataFailed, LoginRespData);
				}
				CheckVersionDelegatesMap.Remove(DelegateGuid);
			}
		}



		break;

	default:


		break;
	}



}

bool UXGSampleClientSubsystem::LoadXGLoginServerIPAndPort()
{
	XGLoginServerIP = UXGSampleClientSettings::GetXGLoginServerSettings()->ServerIP;
	XGLoginServerPort = UXGSampleClientSettings::GetXGLoginServerSettings()->ServerPort;

	bool bServerIP = true && !XGLoginServerIP.IsEmpty();
	bool bServerPort = XGLoginServerPort.IsNumeric();

	if (bServerIP && bServerPort)
	{
		UE_LOG(LogXGSampleClient, Log, TEXT("[%s],ServerIP: %s"), *FString(__FUNCTION__), *XGLoginServerIP);
		UE_LOG(LogXGSampleClient, Log, TEXT("[%s],ServerPort: %s"), *FString(__FUNCTION__), *XGLoginServerPort);

		return true;
	}

	return false;

}

bool UXGSampleClientSubsystem::LoadMD5TokenAndNum(FString& Token, int32& HashNum)
{
	if (UXGSampleClientSettings* LoginClientSettings = UXGSampleClientSettings::GetXGLoginServerSettings())
	{
		Token = LoginClientSettings->Token;
		HashNum = LoginClientSettings->HashNum;
		return true;
	}
	return false;
}

FString UXGSampleClientSubsystem::GenerateToken()
{
	FString ClientToken = TEXT("123456");

	int32  MD5Num = 1;

	if (LoadMD5TokenAndNum(ClientToken, MD5Num))
	{
		for (int32 Index = 0; Index < MD5Num; Index++)
		{
			ClientToken = FMD5::HashAnsiString(*ClientToken);
		}
	}

	return ClientToken;
}

FString UXGSampleClientSubsystem::MD5HashPassword(FString InPassword)
{

	FString	HashString = FMD5::HashAnsiString(*InPassword);
	return HashString;


}
