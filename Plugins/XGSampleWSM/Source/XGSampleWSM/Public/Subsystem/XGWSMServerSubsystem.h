// Copyright 2023 Xiao Gang. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WebSocketNetworkingDelegates.h"
#include "IWebSocketServer.h"
#include "Connection/XGWSMSeverConnection.h"
#include "XGWSMServerSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGWSMServerConnectionConnectedSuccessCallBack, FGuid, ServerConnectionID, FGuid, ClientConnectionID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXGWSServerConnectionConnectedErrorCallBack, FGuid, ServerConnectionID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGWSMServerConnectionReceiveMessageCallBack, FGuid, ServerConnectionID, FString, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXGWSMServerConnectionClosedCallBack, FGuid, ServerConnectionID);



class IWebSocketServer;
class INetworkingWebSocket;

UCLASS()
class XGSAMPLEWSM_API UXGWSMServerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartWebSocketServer", Keywords = "XG Web Socket Message WSM Server Start"), Category = "XGWebSocketMessage|SocketServer")
	bool StartWebSocketServer(int32 InServerPort);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopWebSocketServer", Keywords = "XG Web Socket Message WSM Stop"), Category = "XGWebSocketMessage|SocketServer")
	void StopWebSocketServer();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TryGetDSPort", Keywords = "XG Web Socket Message WSM Delicated Server DS Port"), Category = "XGWebSocketMessage|SocketServer")
	bool TryGetDSPort(int32& OutPort);

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SendMessageToAllClient",
		Keywords = "XG Web Socket Message WSM Send All Client"), Category = "XGWebSocketMessage|SocketServer")
	void SendMessageToAllClient(FString InMessage);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SendMessageToPointedClient",
		Keywords = "XG Web Socket Message WSM Send Pointed Client"), Category = "XGWebSocketMessage|SocketServer")
	void SendMessageToPointedClient(FGuid InTargetServerConnectionID, FString InMessage);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnlinkPointedServerConnection",
		Keywords = "XG Web Socket Message WSM Unlink Pointed Client Connection"), Category = "XGWebSocketMessage|SocketServer")
	void UnlinkPointedServerConnection(FGuid InTargetServerConnectionID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnlinkAllServerConnection",
		Keywords = "XG Web Socket Message WSM Unlink  All Client Connection"), Category = "XGWebSocketMessage|SocketServer")
	void UnlinkAllServerConnection();

	bool CheckServerConnectionIsValid(FGuid InTargetServerConnectionID);


protected:

	void ReleasePointedServerConnection(FGuid InTargetServerConnectionID);

	void ReleaseAllServerConnections();


protected:

	void OnWebSocketClientConnected(INetworkingWebSocket* InWebSocket);

	EWebsocketConnectionFilterResult OnWebSocketFilterConnectionCallback(FString InOrigin, FString InClientIP);

	bool IsWebSocketServerRunning();

	void OnConnectedCallBack(FGuid InTargetServerConnectionID, FGuid InClientConnectionID);

	void OnErrorCallBack(FGuid InTargetServerConnectionID);

	void OnReceiveCallBack(void* Data, int32 DataSize, FGuid InTargetServerConnectionID);

	void OnClosedCallBack(FGuid InTargetServerConnectionID);

protected:

	void OnMessageDispatch(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);

	void OnClientCallServerInit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);

	void OnClientTick(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);

	void OnClientMessageToServer(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);

	void OnClientReqQuit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);

	void OnClientAllowServerQuit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage);


protected:

	void TickServerConnection(float InDeltaTime);

public:

	UPROPERTY(BlueprintAssignable)
	FXGWSMServerConnectionConnectedSuccessCallBack ServerConnectionConnectedSuccessDelegate;

	UPROPERTY(BlueprintAssignable)
	FXGWSServerConnectionConnectedErrorCallBack	ServerConnectionConnectedErrorDelegate;

	UPROPERTY(BlueprintAssignable)
	FXGWSMServerConnectionReceiveMessageCallBack ServerConnectionReceiveMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FXGWSMServerConnectionClosedCallBack ServerConnectionClosedDelegate;

private:



	TUniquePtr<IWebSocketServer> WebSocketServer;


	TMap<FGuid, FXGWSMSeverConnection> ServerConnections;




};