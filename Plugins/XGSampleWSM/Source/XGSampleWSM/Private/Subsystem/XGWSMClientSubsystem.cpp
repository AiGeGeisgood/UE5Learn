// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGWSMClientSubsystem.h"


#include "WebSocketsModule.h"
#include "IWebSocket.h"

#include "JsonObjectConverter.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "LogXGWSM.h"
#include "XGWSMUtil.h"




#pragma region IniteSubsystem
bool UXGWSMClientSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGWSMClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{


	Super::Initialize(Collection);
}

void UXGWSMClientSubsystem::Deinitialize()
{

	UnLinkAllClientConnection();
	Super::Deinitialize();
}

void UXGWSMClientSubsystem::Tick(float DeltaTime)
{
	TickClientConnection(DeltaTime);


}

bool UXGWSMClientSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId UXGWSMClientSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGWSMClientSubsystem, STATGROUP_Tickables);
}
void UXGWSMClientSubsystem::LinkSocketServer(const FString& InServerURL, FGuid& OutClientConnectionID)
{
	OutClientConnectionID = FGuid::NewGuid();

	FXGWSMClientConnection SocketClientConnection(OutClientConnectionID);

	ClientConnections.Add(OutClientConnectionID, MoveTemp(SocketClientConnection));

	FString InServerProtocol = TEXT("");

	TMap<FString, FString> InServerUpgradeHeaders;

	AsyncTask(ENamedThreads::GameThread, [InServerURL, InServerProtocol, InServerUpgradeHeaders, OutClientConnectionID, this]() {

		CreateSocket(InServerURL, InServerProtocol, InServerUpgradeHeaders, OutClientConnectionID);

		});




}
void UXGWSMClientSubsystem::UnLinkPointedClientConnection(const FGuid& InTargetClientConnectionID)
{
	UE_LOG(LogXGSampeWSMClient, Display, TEXT("[%s],ClientConnectionID:[%s]"), *FString(__FUNCTION__), *(InTargetClientConnectionID.ToString()));

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{
		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::PrepareToQuit;

		FXGWSMMessage WebSocketMessage;

		WebSocketMessage.Code = 0;
		WebSocketMessage.Action = EXGWSMActionType::ClientReqQuit;

		WebSocketMessage.ClientConnectionID = ClientConnectionPtr->ClientConnectionID;
		WebSocketMessage.ServerConnectionID = ClientConnectionPtr->ServerConnectionID;
		WebSocketMessage.Description = TEXT("");

		WebSocketMessage.Data = TEXT("");

		ClientConnectionPtr->Send(WebSocketMessage);
	}

	//ClearPointedClientConnectionResources(InTargetClientConnectionID);

}
void UXGWSMClientSubsystem::UnLinkAllClientConnection()
{
	UE_LOG(LogXGSampeWSMClient, Warning, TEXT("%s"), *FString(__FUNCTION__));

	for (auto& TmpClientConnection : ClientConnections)
	{
		UnLinkPointedClientConnection(TmpClientConnection.Key);
	}
	//ClearAllClientConnectionResources();

}
void UXGWSMClientSubsystem::SendMessageToServer(const FString& InMessage, const FGuid& InTargetClientConnectionID)
{

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{

		FXGWSMMessage WebSocketMessage;

		WebSocketMessage.Code = 0;
		WebSocketMessage.Action = EXGWSMActionType::ClientMessageToServer;

		WebSocketMessage.ClientConnectionID = ClientConnectionPtr->ClientConnectionID;
		WebSocketMessage.ServerConnectionID = ClientConnectionPtr->ServerConnectionID;

		WebSocketMessage.Description = TEXT("Pass user-defined information");
		WebSocketMessage.Data = InMessage;

		ClientConnectionPtr->Send(WebSocketMessage);


	}


}
void UXGWSMClientSubsystem::CreateSocket(const FString& InServerURL, const FString& InServerProtocol, const TMap<FString, FString>& InServerUpgradeHeaders, const FGuid InTargetClientConnectionID)
{
	FModuleManager::Get().LoadModuleChecked("WebSockets");

	FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID);

	TSharedPtr<IWebSocket> WebClientSocket = FWebSocketsModule::Get().CreateWebSocket(InServerURL, InServerProtocol);

	if (ClientConnectionPtr)
	{
		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::None;

		ClientConnectionPtr->ClientWebSocket = WebClientSocket;


		WebClientSocket->OnConnected().AddUObject(this, &UXGWSMClientSubsystem::OnConnected, InTargetClientConnectionID);

		WebClientSocket->OnConnectionError().AddUObject(this, &UXGWSMClientSubsystem::OnConnectionError, InTargetClientConnectionID);

		WebClientSocket->OnClosed().AddUObject(this, &UXGWSMClientSubsystem::OnClosed, InTargetClientConnectionID);

		WebClientSocket->OnMessage().AddUObject(this, &UXGWSMClientSubsystem::OnMessage, InTargetClientConnectionID);

		WebClientSocket->OnMessageSent().AddUObject(this, &UXGWSMClientSubsystem::OnMessageSent, InTargetClientConnectionID);

		WebClientSocket->OnBinaryMessage().AddUObject(this, &UXGWSMClientSubsystem::OnBinaryMessage, InTargetClientConnectionID);

		WebClientSocket->Connect();


	}



}
void UXGWSMClientSubsystem::ClearPointedClientConnectionResources(const FGuid& InTargetClientConnectionID)
{
	ClientConnections.Remove(InTargetClientConnectionID);

	ClientConnections.Remove(InTargetClientConnectionID);
}
void UXGWSMClientSubsystem::ClearAllClientConnectionResources()
{
	ClientConnections.Empty();

	ClientConnectionDatas.Empty();

}
void UXGWSMClientSubsystem::CallInitServerConnection(FGuid InTargetClientConnectionID)
{
	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{

		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::InitServerConnection;

		FXGWSMMessage WebSocketMessage;

		WebSocketMessage.Code = 0;
		WebSocketMessage.Action = EXGWSMActionType::ClientCallServerInit;

		WebSocketMessage.ClientConnectionID = InTargetClientConnectionID;
		WebSocketMessage.Description = TEXT("ClientCallServerInit");

		WebSocketMessage.Data = TEXT("");

		ClientConnectionPtr->Send(WebSocketMessage);
	}


}
void UXGWSMClientSubsystem::OnConnected(FGuid InTargetClientConnectionID)
{
	UE_LOG(LogXGSampeWSMClient, Display, TEXT("[%s],TargetClientConnectionID:[%s]"), *FString(__FUNCTION__), *(InTargetClientConnectionID.ToString()));

	CallInitServerConnection(InTargetClientConnectionID);



}
void UXGWSMClientSubsystem::OnConnectionError(const FString& Error, FGuid InTargetClientConnectionID)
{
	UE_LOG(LogXGSampeWSMClient, Warning, TEXT("[%s] Error:[%s],TargetClientConnectionID:[%s]"), *FString(__FUNCTION__), *Error, *(InTargetClientConnectionID.ToString()));

	FXGWSMClientConnectionConnectedErrorCallBack TempDelegate = ClientConnectionConnectedErrorCallBack;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(Error, InTargetClientConnectionID);

		});


	ClearPointedClientConnectionResources(InTargetClientConnectionID);



}
void UXGWSMClientSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean, FGuid InTargetClientConnectionID)
{
	UE_LOG(LogXGSampeWSMClient, Warning, TEXT("[%s],TargetClientConnectionID:[%s],StatusCode:[%d],Reason:[%s],bWasClean:[%s]"),
		*FString(__FUNCTION__),
		*(InTargetClientConnectionID.ToString()),
		StatusCode,
		*Reason,
		bWasClean ? TEXT("true") : TEXT("false"));

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{
		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::Quit;

	}
	FXGWSMClientConnectionClosedCallBack TempDelegate = ClientConnectionClosedCallBack;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(StatusCode, Reason, bWasClean, InTargetClientConnectionID);

		});


	ClearPointedClientConnectionResources(InTargetClientConnectionID);



}
void UXGWSMClientSubsystem::OnMessage(const FString& Message, FGuid InTargetClientConnectionID)
{
	//UE_LOG(LogXGSampeWSMClient, Warning, TEXT("%s"), *FString(__FUNCTION__));

}
void UXGWSMClientSubsystem::OnMessageSent(const FString& MessageString, FGuid InTargetClientConnectionID)
{
	//UE_LOG(LogXGSampeWSMClient, Warning, TEXT("%s"), *FString(__FUNCTION__));

}
void UXGWSMClientSubsystem::OnBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment, FGuid InTargetClientConnectionID)
{

	//UE_LOG(LogXGSampeWSMClient, Warning, TEXT("[%s],DataSize:[%d],bIsLastFragment:[%s]"), *FString(__FUNCTION__), Size, bIsLastFragment?TEXT("true"): TEXT("false"));
	if (bIsLastFragment)
	{
		FString JsonMessage = TEXT("");

		if (TArray<uint8>* ClientConnectionData = ClientConnectionDatas.Find(InTargetClientConnectionID))
		{
			ClientConnectionData->Append((const uint8*)Data, Size);

			JsonMessage = UXGWSMUtilBPLibrary::XGMessageDecode(*ClientConnectionData);

			UE_LOG(LogXGSampeWSMClient, Log, TEXT("[%s],JsonMessage:[%s]"), *FString(__FUNCTION__), *JsonMessage);

			ClientConnectionDatas.Remove(InTargetClientConnectionID);

		}
		else
		{
			JsonMessage = UXGWSMUtilBPLibrary::XGMessageDecode((uint8*)Data, Size);

			UE_LOG(LogXGSampeWSMClient, Log, TEXT("[%s],JsonMessage:[%s]"), *FString(__FUNCTION__), *JsonMessage);

		}


		FXGWSMMessage Message;

		bool bParse = FJsonObjectConverter::JsonObjectStringToUStruct(JsonMessage, &Message);

		if (bParse && Message.Code == 0)
		{
			OnMessageDispatch(InTargetClientConnectionID, Message);

		}
		else
		{
			UE_LOG(LogXGSampeWSMClient, Error, TEXT("[%s],bParse is wrong:[%s]"), *FString(__FUNCTION__), *JsonMessage);
		}

	}
	else
	{
		if (TArray<uint8>* ClientConnectionData = ClientConnectionDatas.Find(InTargetClientConnectionID))
		{

			ClientConnectionData->Append((const uint8*)Data, Size);
		}
		else
		{
			TArray<uint8> NewClientConnectionData;

			NewClientConnectionData.Append((const uint8*)Data, Size);

			ClientConnectionDatas.Add(InTargetClientConnectionID, NewClientConnectionData);
		}

	}







}
void UXGWSMClientSubsystem::OnMessageDispatch(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{

	switch (InMessage.Action)
	{

	case EXGWSMActionType::None:
		break;


	case EXGWSMActionType::ServerResponseClientInit:

		OnServerResponseClientInit(InTargetClientConnectionID, InMessage);

		break;

	case EXGWSMActionType::ServerResponseTick:

		OnServerResponseTick(InTargetClientConnectionID, InMessage);

		break;

	case EXGWSMActionType::ServerMessageToClient:

		OnServerMessageToClient(InTargetClientConnectionID, InMessage);
		break;

	case EXGWSMActionType::ServerReqQuit:

		OnServerReqQuit(InTargetClientConnectionID, InMessage);


		break;
	case EXGWSMActionType::ServerAllowClientQuit:

		OnServerAllowClientQuit(InTargetClientConnectionID, InMessage);

		break;

	default:
		break;
	}




}
void UXGWSMClientSubsystem::OnServerResponseClientInit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{

		ClientConnectionPtr->ServerConnectionID = InMessage.ServerConnectionID;

		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::Tick;


		FGuid InServerConnectionID = InMessage.ServerConnectionID;

		FXGWSMClientConnectionConnectedSuccessCallBack TempDelegate = ClientConnectionConnectedSuccessCallBack;

		AsyncTask(ENamedThreads::GameThread, [=]() {

			TempDelegate.Broadcast(InServerConnectionID, InTargetClientConnectionID);

			});


	}





}
void UXGWSMClientSubsystem::OnServerResponseTick(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{
		if (ClientConnectionPtr->ClientConnectionStatus == EXGWSMClientConnectionStatus::Tick)
		{
			ClientConnectionPtr->UpdateIntervalTime();
		}
	}


}
void UXGWSMClientSubsystem::OnServerMessageToClient(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{

	FXGWSMClientConnectionReceiveMessageCallBack TempDelegate = ClientConnectionReceiveMessageCallBack;

	AsyncTask(ENamedThreads::GameThread, [TempDelegate, InMessage, InTargetClientConnectionID]() {

		TempDelegate.Broadcast(InTargetClientConnectionID, InMessage.Data);

		});


}
void UXGWSMClientSubsystem::OnServerReqQuit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{
	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{
		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::Quit;

		FXGWSMMessage WebSocketMessage;
		WebSocketMessage.Code = 0;
		WebSocketMessage.Action = EXGWSMActionType::ClientAllowServerQuit;
		WebSocketMessage.ClientConnectionID = InTargetClientConnectionID;
		WebSocketMessage.Description = TEXT("");
		WebSocketMessage.Data = TEXT("");

		ClientConnectionPtr->Send(WebSocketMessage);

		//ClearPointedClientConnectionResources(InTargetClientConnectionID);

	}

}
void UXGWSMClientSubsystem::OnServerAllowClientQuit(const FGuid& InTargetClientConnectionID, const FXGWSMMessage& InMessage)
{

	if (FXGWSMClientConnection* ClientConnectionPtr = ClientConnections.Find(InTargetClientConnectionID))
	{
		ClientConnectionPtr->ClientConnectionStatus = EXGWSMClientConnectionStatus::Quit;

		ClearPointedClientConnectionResources(InTargetClientConnectionID);

	}

}


void UXGWSMClientSubsystem::TickClientConnection(float InDeltaTime)
{
	TArray<FGuid> OutOfTimeClientConnectionIDs;

	for (auto& TmpClientConnectionPtr : ClientConnections)
	{
		TmpClientConnectionPtr.Value.Tick(InDeltaTime);

		if (TmpClientConnectionPtr.Value.IsOutOfTime())
		{
			TmpClientConnectionPtr.Value.ClientConnectionStatus = EXGWSMClientConnectionStatus::OutOfTime;

			OutOfTimeClientConnectionIDs.Add(TmpClientConnectionPtr.Key);
		}

	}

	for (auto& TmpOutOfTimeClientConnectionID : OutOfTimeClientConnectionIDs)
	{
		ClearPointedClientConnectionResources(TmpOutOfTimeClientConnectionID);
	}


}
#pragma endregion IniteSubsystem