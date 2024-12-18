// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Connection/XGWSMClientConnection.h"
#include "XGWSMClientSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGWSMClientConnectionConnectedSuccessCallBack, FGuid, ServerConnectionID, FGuid, ClientConnectionID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGWSMClientConnectionConnectedErrorCallBack, FString, ErrorMessage, FGuid, ClientConnectionID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGWSMClientConnectionReceiveMessageCallBack, FGuid, ClientConnectionID, FString, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGWSMClientConnectionClosedCallBack, int32, StatusCode, const FString&, Reason, bool, bWasClean, FGuid, ClientConnectionID);


class IWebSocket;


UCLASS()
class XGSAMPLEWSM_API UXGWSMClientSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{

	GENERATED_BODY()

#pragma region IniteSubsystem

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	virtual void Deinitialize()override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

#pragma endregion IniteSubsystem


public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LinkWebSocketServer", 
	KeyWords = "XG Web Socket Message WSM Link Client Server"), 
	Category = "XGWebSocketMessage|SocketClient")
	void LinkSocketServer(const FString& InServerURL, FGuid& OutClientConnectionID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnLinkPointedClientConnection",
	KeyWords = "XG Web Socket Message WSM Unlink Client Connection Server Pointed"), Category = "XGWebSocketMessage|SocketClient")
	void UnLinkPointedClientConnection(const FGuid& InTargetClientConnectionID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnLinkAllClientConnection", 
	KeyWords = "XG Web Socket Message WSM Unlink Client Connection Server"), 
	Category = "XGWebSocketMessage|SocketClient")
	void UnLinkAllClientConnection();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SendMessageToServer",
	KeyWords = "XG Web Socket Message WSM Send Client Server"), 
	Category = "XGWebSocketMessage|SocketClient")
	void SendMessageToServer(const FString& InMessage, const FGuid& InTargetClientConnectionID);


protected:

	void CreateSocket(const FString& InServerURL, 
	const FString& InServerProtocol, 
	const TMap<FString, FString>& InServerUpgradeHeaders, 
	const FGuid InTargetClientConnectionID);

	void ClearPointedClientConnectionResources(const FGuid& InTargetClientConnectionID);

	void ClearAllClientConnectionResources();

	void CallInitServerConnection(FGuid InTargetClientConnectionID);


protected:

	void OnConnected(FGuid InTargetClientConnectionID);

	void OnConnectionError(const FString& Error, FGuid InTargetClientConnectionID);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean, FGuid InTargetClientConnectionID);

	void OnMessage(const FString& Message, FGuid InTargetClientConnectionID);

	void OnMessageSent(const FString& MessageString, FGuid InTargetClientConnectionID);

	void OnBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment, FGuid InTargetClientConnectionID);

protected:

	void OnMessageDispatch(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);

	void OnServerResponseClientInit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);

	void OnServerResponseTick(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);

	void OnServerMessageToClient(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);

	void OnServerReqQuit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);

	void OnServerAllowClientQuit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage);




protected:

	void TickClientConnection(float InDeltaTime);


public:



	UPROPERTY(BlueprintAssignable)
	FXGWSMClientConnectionConnectedSuccessCallBack ClientConnectionConnectedSuccessCallBack;

	UPROPERTY(BlueprintAssignable)
	FXGWSMClientConnectionConnectedErrorCallBack ClientConnectionConnectedErrorCallBack;

	UPROPERTY(BlueprintAssignable)
	FXGWSMClientConnectionReceiveMessageCallBack ClientConnectionReceiveMessageCallBack;

	UPROPERTY(BlueprintAssignable)
	FXGWSMClientConnectionClosedCallBack  ClientConnectionClosedCallBack;

private:

	TMap<FGuid, FXGWSMClientConnection> ClientConnections;

	TMap<FGuid, TArray<uint8>> ClientConnectionDatas;


};