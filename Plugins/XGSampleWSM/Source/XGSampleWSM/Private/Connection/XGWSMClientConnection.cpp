// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGWSMClientConnection.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"


#include "XGWSMUtil.h"

FXGWSMClientConnection::FXGWSMClientConnection(FGuid InClientConnectionID)
	: ClientWebSocket(nullptr),
	ClientConnectionID(InClientConnectionID),
	ServerConnectionID(FGuid()),
	ClientConnectionStatus(EXGWSMClientConnectionStatus::None)
{



}

FXGWSMClientConnection::FXGWSMClientConnection(TSharedPtr<IWebSocket> InClientWebSocket) 
: ClientWebSocket(InClientWebSocket),
ClientConnectionID(FGuid::NewGuid()),
ServerConnectionID(FGuid()),
ClientConnectionStatus(EXGWSMClientConnectionStatus::None)
{



}

FXGWSMClientConnection::FXGWSMClientConnection(FXGWSMClientConnection&& WebSocketConnection)
	: ClientWebSocket(WebSocketConnection.ClientWebSocket),
	ClientConnectionID(WebSocketConnection.ClientConnectionID),
	ServerConnectionID(WebSocketConnection.ServerConnectionID),
	ClientConnectionStatus(WebSocketConnection.ClientConnectionStatus)
{



}

FXGWSMClientConnection::~FXGWSMClientConnection()
{

	if (ClientWebSocket.IsValid() && ClientWebSocket->IsConnected())
	{
		ClientWebSocket->Close();

	}

	ClientWebSocket.Reset();

}

void FXGWSMClientConnection::Send(const FXGWSMMessage& InMessage)
{
	if (ClientWebSocket.IsValid() && ClientWebSocket->IsConnected())
	{
		FString JsonStr;
		FJsonObjectConverter::UStructToJsonObjectString(InMessage, JsonStr);

		TArray<uint8>  MessageData = UXGWSMUtilBPLibrary::XGMessageEncode(JsonStr);

		ClientWebSocket->Send(MessageData.GetData(), MessageData.Num(), false);

	}


}

void FXGWSMClientConnection::UpdateIntervalTime()
{

	IntervalTime = 0.f;

}

bool FXGWSMClientConnection::IsOutOfTime()
{
	if (IntervalTime > 10.f)
	{
		return true;
	}

	return false;
}

void FXGWSMClientConnection::Tick(float InDeltaTime)
{

	IntervalTime += InDeltaTime;

	HeartTime += InDeltaTime;


	if (HeartTime > HeartCheckTime)
	{
		HeartTime = 0.f;
		FXGWSMMessage TickMessage;
		TickMessage.Code = 0;
		TickMessage.Action = EXGWSMActionType::ClientCallTick;

		TickMessage.ServerConnectionID = ServerConnectionID;
		TickMessage.ClientConnectionID = ClientConnectionID;

		TickMessage.Description = TEXT("ClientCallTick");
		TickMessage.Data = TEXT("");

		Send(TickMessage);

	}




}
