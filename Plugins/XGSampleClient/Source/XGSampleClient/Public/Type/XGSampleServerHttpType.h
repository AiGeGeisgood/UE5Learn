#pragma once

#include "CoreMinimal.h"
#include "XGSampleServerHttpType.generated.h"



UENUM(BlueprintType)
enum class EXGSampleServerResult : uint8
{
	Succeeded,
	Failed,


	ConnectSQLFailed,
	GetWrongSQLResult,

	ParseServerIpAndPortFailed,
	ParseServerResponseFailed,
	ParseServerResponseDataFailed,

	ParseClientRequestFailed,
	ParseClientRequestDataFailed,

	NetError,


	IllToken,
	IllVersion,
	IllReqType,
	IllegalUserName,
	IllegaPassword,
	IllegaPhone,
	IllegalEMail,
	IllMachineID,
	IllConsumeMoney,
	IllChargeMoney,

	RepetitiveName,
	WrongUserNameOrPassword,
	WrongMachineID,
	NoUser,
	NoMachineID,
	NoEnoughBalance,
	NoManagerAuthority,
	UnKnownReqType,


	UnKnown,
	MAX

};




