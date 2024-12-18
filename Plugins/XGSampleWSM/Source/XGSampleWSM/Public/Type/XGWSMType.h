
#pragma once

#include "CoreMinimal.h"
#include "XGWSMType.generated.h"

UENUM()
enum class EXGWSMActionType : uint8
{
	None,

	ClientCallServerInit,
	ServerResponseClientInit,

	ClientCallTick,
	ServerResponseTick,

	ClientMessageToServer,
	ServerMessageToClient,

	ClientReqQuit,
	ServerAllowClientQuit,

	ServerReqQuit,
	ClientAllowServerQuit,

	Max,

};


UENUM()
enum class EXGWSMClientConnectionStatus : uint8
{
	None,
	InitServerConnection,

	Tick,

	PrepareToQuit,
	Quit,

	OutOfTime,

	Max,

};


UENUM()
enum class EXGWSMServerConnectionStatus : uint8
{
	None,

	WaitClientToInit,
	Tick,

	PrepareToQuit,
	Quit,

	OutOfTime,

	Max,

};

USTRUCT()
struct  FXGWSMMessage
{

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	int32 Code = -1;

	UPROPERTY()
	EXGWSMActionType Action = EXGWSMActionType::None;

	UPROPERTY()
	FGuid ClientConnectionID;

	UPROPERTY()
	FGuid ServerConnectionID;

	UPROPERTY()
	FString Description = TEXT("");

	UPROPERTY()
	FString Data = TEXT("");

};


