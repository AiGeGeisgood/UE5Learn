// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGWSMServerSubsystem.h"

#include "INetworkingWebSocket.h"
#include "IWebSocketNetworkingModule.h"
#include "IWebSocketServer.h"

#include "JsonObjectConverter.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Engine/World.h"
#include "Engine/Engine.h"




#include "LogXGWSM.h"
#include "XGWSMUtil.h"


#pragma region IniteSubsystem
bool UXGWSMServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGWSMServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UXGWSMServerSubsystem::Deinitialize()
{
	StopWebSocketServer();
	Super::Deinitialize();
}

void UXGWSMServerSubsystem::Tick(float DeltaTime)
{

	if (IsWebSocketServerRunning())
	{
		WebSocketServer->Tick();
	}

	 TickServerConnection(DeltaTime);


}

bool UXGWSMServerSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGWSMServerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGWSMServerSubsystem, STATGROUP_Tickables);
}
bool UXGWSMServerSubsystem::StartWebSocketServer(int32 InServerPort)
{
	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	if (WebSocketServer && WebSocketServer.IsValid())
	{
		UE_LOG(LogXGSampeWSMServer, Warning, TEXT("%s:WebSocketServer is running"), *FString(__FUNCTION__));

		return false;
	}

	FWebSocketClientConnectedCallBack ClientConnectedCallBack;

	ClientConnectedCallBack.BindUObject(this, &UXGWSMServerSubsystem::OnWebSocketClientConnected);

	FWebSocketFilterConnectionCallback  FilterConnectionCallback;

	FilterConnectionCallback.BindUObject(this, &UXGWSMServerSubsystem::OnWebSocketFilterConnectionCallback);

	WebSocketServer = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();

	if (!WebSocketServer)
	{
		UE_LOG(LogXGSampeWSMServer, Warning, TEXT("%s:WebSocketServer Init Failed "), *FString(__FUNCTION__));

		WebSocketServer.Reset();

		return false;
	}



	WebSocketServer->SetFilterConnectionCallback(FilterConnectionCallback);

	if (!WebSocketServer->Init(InServerPort, ClientConnectedCallBack))
	{

		UE_LOG(LogXGSampeWSMServer, Warning, TEXT("%s:WebSocketServer Init Failed "), *FString(__FUNCTION__));

		WebSocketServer.Reset();

		return false;
	}




	return true;
}
void UXGWSMServerSubsystem::StopWebSocketServer()
{

	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	//需要通知所有客户端 服务器退出了
	UnlinkAllServerConnection();

	if (IsWebSocketServerRunning())
	{
		WebSocketServer.Reset();
	}



}
bool UXGWSMServerSubsystem::TryGetDSPort(int32& OutPort)
{

	UWorld* WorldContextObject = GetWorld();

	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		if (WorldContextObject)
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				OutPort = World->URL.Port;

				FString Port = FString::FromInt(World->URL.Port);

				UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s--URL.Port:%s"), *FString(__FUNCTION__), *Port);
			}

		}
	}

	return false;
}
void UXGWSMServerSubsystem::SendMessageToAllClient(FString InMessage)
{
	FXGWSMMessage WSMMessage;

	WSMMessage.Code = 0;

	WSMMessage.Action = EXGWSMActionType::ServerMessageToClient;

	WSMMessage.Data = InMessage;


	for (auto& TmpSocketConnection : ServerConnections)
	{
		WSMMessage.ClientConnectionID = TmpSocketConnection.Value.ClientConnectionID;

		WSMMessage.ServerConnectionID = TmpSocketConnection.Value.ServerConnectionID;

		TmpSocketConnection.Value.Send(WSMMessage);
	}


}
void UXGWSMServerSubsystem::SendMessageToPointedClient(FGuid InTargetServerConnectionID, FString InMessage)
{
	FXGWSMMessage WSMMessage;

	WSMMessage.Code = 0;

	WSMMessage.Action = EXGWSMActionType::ServerMessageToClient;

	WSMMessage.Data = InMessage;

	WSMMessage.ServerConnectionID = InTargetServerConnectionID;

	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		WSMMessage.ClientConnectionID = SeverConnectionPtr->ClientConnectionID;

		SeverConnectionPtr->Send(WSMMessage);
	}



}
void UXGWSMServerSubsystem::UnlinkPointedServerConnection(FGuid InTargetServerConnectionID)
{
	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		SeverConnectionPtr->ServerConnectionStatus = EXGWSMServerConnectionStatus::PrepareToQuit;

		FXGWSMMessage WSMMessage;

		WSMMessage.Code = 0;

		WSMMessage.Action = EXGWSMActionType::ServerReqQuit;

		WSMMessage.ClientConnectionID = SeverConnectionPtr->ClientConnectionID;

		WSMMessage.ServerConnectionID = SeverConnectionPtr->ServerConnectionID;

		WSMMessage.Data = TEXT("");

		SeverConnectionPtr->Send(WSMMessage);

	}


	//ReleasePointedServerConnection(InTargetServerConnectionID);

}
void UXGWSMServerSubsystem::UnlinkAllServerConnection()
{

	for (auto& TmpServerConnection : ServerConnections)
	{
		UnlinkPointedServerConnection(TmpServerConnection.Key);
	}


}
bool UXGWSMServerSubsystem::CheckServerConnectionIsValid(FGuid InTargetServerConnectionID)
{

	if (IsWebSocketServerRunning())
	{
		FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID);

		if (SeverConnectionPtr)
		{
			return true;
		}
	}

	return false;
}
void UXGWSMServerSubsystem::ReleasePointedServerConnection(FGuid InTargetServerConnectionID)
{
	int32 RemoveNum = ServerConnections.Remove(InTargetServerConnectionID);

	if (RemoveNum == 1)
	{
		FXGWSMServerConnectionClosedCallBack TempDeleage = ServerConnectionClosedDelegate;

		AsyncTask(ENamedThreads::GameThread, [TempDeleage, InTargetServerConnectionID]() {

			TempDeleage.Broadcast(InTargetServerConnectionID);

			});
	}


}
void UXGWSMServerSubsystem::ReleaseAllServerConnections()
{
	for (auto& TmpServerConnection : ServerConnections)
	{
		FGuid ServerConnectionIDToRealease = TmpServerConnection.Key;
		FXGWSMServerConnectionClosedCallBack TempDeleage = ServerConnectionClosedDelegate;

		AsyncTask(ENamedThreads::GameThread, [TempDeleage, ServerConnectionIDToRealease]() {

			TempDeleage.Broadcast(ServerConnectionIDToRealease);

			});
	}

	ServerConnections.Empty();

}
void UXGWSMServerSubsystem::OnWebSocketClientConnected(INetworkingWebSocket* InWebSocket)
{
	if (InWebSocket)
	{
		FXGWSMSeverConnection XGWebSocketConnection = FXGWSMSeverConnection{ InWebSocket };

		XGWebSocketConnection.ServerConnectionStatus = EXGWSMServerConnectionStatus::WaitClientToInit;

		//FWebSocketInfoCallBack ConnectedCallBack;

		//ConnectedCallBack.BindUObject(this, &UXGWSMServerSubsystem::OnConnectedCallBack, XGWebSocketConnection.ServerConnectionID);
		//InWebSocket->SetConnectedCallBack(ConnectedCallBack);


		FWebSocketInfoCallBack ErrorCallBack;
		ErrorCallBack.BindUObject(this, &UXGWSMServerSubsystem::OnErrorCallBack, XGWebSocketConnection.ServerConnectionID);
		InWebSocket->SetErrorCallBack(ErrorCallBack);


		FWebSocketPacketReceivedCallBack ReceiveCallBack;
		ReceiveCallBack.BindUObject(this, &UXGWSMServerSubsystem::OnReceiveCallBack, XGWebSocketConnection.ServerConnectionID);
		InWebSocket->SetReceiveCallBack(ReceiveCallBack);


		FWebSocketInfoCallBack CloseCallback;
		CloseCallback.BindUObject(this, &UXGWSMServerSubsystem::OnClosedCallBack, XGWebSocketConnection.ServerConnectionID);
		InWebSocket->SetSocketClosedCallBack(CloseCallback);

		ServerConnections.Add(XGWebSocketConnection.ServerConnectionID, MoveTemp(XGWebSocketConnection));

		UE_LOG(LogXGSampeWSMServer, Display, TEXT("[%s],XGWebSocketID:[%s]"), *FString(__FUNCTION__), *(XGWebSocketConnection.ServerConnectionID.ToString()));

	}
	else
	{
		UE_LOG(LogXGSampeWSMServer, Error, TEXT("[%s],InWebSocket is nullptr !"), *FString(__FUNCTION__));
	}





}
EWebsocketConnectionFilterResult UXGWSMServerSubsystem::OnWebSocketFilterConnectionCallback(FString InOrigin, FString InClientIP)
{
	UE_LOG(LogXGSampeWSMServer, Error, TEXT("[%s],Origin:[%s],ClientIP:[%s]!"), *FString(__FUNCTION__), *InOrigin, *InClientIP);

	return EWebsocketConnectionFilterResult::ConnectionAccepted;

}
bool UXGWSMServerSubsystem::IsWebSocketServerRunning()
{


	if (WebSocketServer && WebSocketServer.IsValid())
	{
		return true;
	}
	return false;
}
void UXGWSMServerSubsystem::OnConnectedCallBack(FGuid InTargetServerConnectionID, FGuid InClientConnectionID)
{
	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s--InTargetServerConnectionID:%s"), *FString(__FUNCTION__), *InTargetServerConnectionID.ToString());

	FXGWSMServerConnectionConnectedSuccessCallBack TempDeleage = ServerConnectionConnectedSuccessDelegate;

	AsyncTask(ENamedThreads::GameThread, [TempDeleage, InTargetServerConnectionID, InClientConnectionID]() {

		TempDeleage.Broadcast(InTargetServerConnectionID, InClientConnectionID);
		});



}
void UXGWSMServerSubsystem::OnErrorCallBack(FGuid InTargetServerConnectionID)
{
	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s--InTargetServerConnectionID:%s"), *FString(__FUNCTION__), *InTargetServerConnectionID.ToString());

	FXGWSServerConnectionConnectedErrorCallBack TempDeleage = ServerConnectionConnectedErrorDelegate;

	AsyncTask(ENamedThreads::GameThread, [TempDeleage, InTargetServerConnectionID]() {

		TempDeleage.Broadcast(InTargetServerConnectionID);

		});

}
void UXGWSMServerSubsystem::OnReceiveCallBack(void* Data, int32 DataSize, FGuid InTargetServerConnectionID)
{
	FString MessageStr = UXGWSMUtilBPLibrary::XGMessageDecode((uint8*)Data, DataSize);


	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s--InTargetServerConnectionID:%s Message:[%s]"), *FString(__FUNCTION__), *InTargetServerConnectionID.ToString(), *MessageStr);

	FXGWSMMessage Message;

	bool bParse = FJsonObjectConverter::JsonObjectStringToUStruct(MessageStr, &Message);

	if (bParse && Message.Code == 0)
	{

		OnMessageDispatch(InTargetServerConnectionID, Message);

	}
	else
	{

		UE_LOG(LogXGSampeWSMServer, Error, TEXT("[%s],InTargetServerConnectionID:[%s],Message:[%s] is illegal !"),
		*FString(__FUNCTION__), *InTargetServerConnectionID.ToString(), *MessageStr);

		UnlinkPointedServerConnection(InTargetServerConnectionID);

	}



}
void UXGWSMServerSubsystem::OnClosedCallBack(FGuid InTargetServerConnectionID)
{
	UE_LOG(LogXGSampeWSMServer, Display, TEXT("%s--InTargetServerConnectionID:%s"), *FString(__FUNCTION__), *InTargetServerConnectionID.ToString());

	ReleasePointedServerConnection(InTargetServerConnectionID);

}
void UXGWSMServerSubsystem::OnMessageDispatch(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{
	switch (InMessage.Action)
	{
	case EXGWSMActionType::None:

		break;

	case EXGWSMActionType::ClientCallServerInit:

		OnClientCallServerInit(InTargetServerConnectionID, InMessage);

		break;

	case EXGWSMActionType::ClientCallTick:

		OnClientTick(InTargetServerConnectionID, InMessage);

		break;

	case EXGWSMActionType::ClientMessageToServer:

		OnClientMessageToServer(InTargetServerConnectionID, InMessage);

		break;

	case EXGWSMActionType::ClientReqQuit:

		OnClientReqQuit(InTargetServerConnectionID, InMessage);

		break;

	case EXGWSMActionType::ClientAllowServerQuit:

		OnClientAllowServerQuit(InTargetServerConnectionID, InMessage);

		break;

	default:
		break;
	}

}
void UXGWSMServerSubsystem::OnClientCallServerInit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{
	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		SeverConnectionPtr->ClientConnectionID = InMessage.ClientConnectionID;
		SeverConnectionPtr->ServerConnectionStatus = EXGWSMServerConnectionStatus::Tick;

		FXGWSMMessage CallBackMessage;

		CallBackMessage.Code = 0;
		CallBackMessage.Action = EXGWSMActionType::ServerResponseClientInit;
		CallBackMessage.ClientConnectionID = SeverConnectionPtr->ClientConnectionID;
		CallBackMessage.ServerConnectionID = InTargetServerConnectionID;

		CallBackMessage.Data = TEXT("");

		SeverConnectionPtr->Send(CallBackMessage);

		OnConnectedCallBack(InTargetServerConnectionID, SeverConnectionPtr->ClientConnectionID);

	}



}
void UXGWSMServerSubsystem::OnClientTick(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{
	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		if (SeverConnectionPtr->ServerConnectionStatus == EXGWSMServerConnectionStatus::Tick)
		{

			SeverConnectionPtr->UpdateIntervalTime();

			FXGWSMMessage CallBackMessage;

			CallBackMessage.Code = 0;

			CallBackMessage.Action = EXGWSMActionType::ServerResponseTick;
			CallBackMessage.ClientConnectionID = SeverConnectionPtr->ClientConnectionID;
			CallBackMessage.ServerConnectionID = InTargetServerConnectionID;

			CallBackMessage.Data = TEXT("");

			SeverConnectionPtr->Send(CallBackMessage);
		}

	}


}
void UXGWSMServerSubsystem::OnClientMessageToServer(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{
	FXGWSMServerConnectionReceiveMessageCallBack TempDeleage = ServerConnectionReceiveMessageDelegate;

	AsyncTask(ENamedThreads::GameThread, [TempDeleage, InTargetServerConnectionID, InMessage]() {

		TempDeleage.Broadcast(InTargetServerConnectionID, InMessage.Data);

		});

}
void UXGWSMServerSubsystem::OnClientReqQuit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{
	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		SeverConnectionPtr->ServerConnectionStatus = EXGWSMServerConnectionStatus::Quit;

		FXGWSMMessage CallBackMessage;

		CallBackMessage.Code = 0;
		CallBackMessage.Action = EXGWSMActionType::ServerAllowClientQuit;
		CallBackMessage.ClientConnectionID = SeverConnectionPtr->ClientConnectionID;
		CallBackMessage.ServerConnectionID = InTargetServerConnectionID;
		CallBackMessage.Data = TEXT("");

		SeverConnectionPtr->Send(CallBackMessage);

	}

	//ReleasePointedServerConnection(InTargetServerConnectionID);

}
void UXGWSMServerSubsystem::OnClientAllowServerQuit(const FGuid& InTargetServerConnectionID, const FXGWSMMessage& InMessage)
{

	if (FXGWSMSeverConnection* SeverConnectionPtr = ServerConnections.Find(InTargetServerConnectionID))
	{
		SeverConnectionPtr->ServerConnectionStatus = EXGWSMServerConnectionStatus::Quit;
	}
	ReleasePointedServerConnection(InTargetServerConnectionID);

}
void UXGWSMServerSubsystem::TickServerConnection(float InDeltaTime)
{
	TArray<FGuid> OutOfTimeServerConnectionIDs;

	for (auto& TmpServerConnecionPtr : ServerConnections)
	{
		TmpServerConnecionPtr.Value.Tick(InDeltaTime);

		if (TmpServerConnecionPtr.Value.IsOutOfTime())
		{
			TmpServerConnecionPtr.Value.ServerConnectionStatus = EXGWSMServerConnectionStatus::OutOfTime;

			OutOfTimeServerConnectionIDs.Add(TmpServerConnecionPtr.Key);

		}

	}

	for (auto& TmpOutOfTimeServerConnectionID : OutOfTimeServerConnectionIDs)
	{
		ReleasePointedServerConnection(TmpOutOfTimeServerConnectionID);
	}




}
#pragma endregion IniteSubsystem