// Copyright 2023 Xiao Gang. All Rights Reserved.

#include "XGWSMSeverConnection.h"

#include "INetworkingWebSocket.h"

#include "IWebSocketNetworkingModule.h"
#include "JsonObjectConverter.h"

#include "XGWSMUtil.h"



FXGWSMSeverConnection::FXGWSMSeverConnection(INetworkingWebSocket* InServerWebSocket)
	: ServerWebSocket(InServerWebSocket)
	, ClientConnectionID(FGuid())
	, ServerConnectionID(FGuid::NewGuid())
	, ServerConnectionStatus(EXGWSMServerConnectionStatus::None)
{
}

FXGWSMSeverConnection::FXGWSMSeverConnection(FXGWSMSeverConnection&& WebSocketConnection)
	: ClientConnectionID(WebSocketConnection.ClientConnectionID),
	ServerConnectionID(WebSocketConnection.ServerConnectionID),
	ServerConnectionStatus(WebSocketConnection.ServerConnectionStatus)
{

	ServerWebSocket = WebSocketConnection.ServerWebSocket;
	WebSocketConnection.ServerWebSocket = nullptr;


}

FXGWSMSeverConnection::~FXGWSMSeverConnection()
{
	if (ServerWebSocket)
	{
		delete ServerWebSocket;
		ServerWebSocket = nullptr;
	}

}

void FXGWSMSeverConnection::Send(const FXGWSMMessage& InMessage)
{
	FString WSMMessageString;

	FJsonObjectConverter::UStructToJsonObjectString(InMessage, WSMMessageString);

	TArray<uint8> UTF8Array = UXGWSMUtilBPLibrary::XGMessageEncode(WSMMessageString);

	ServerWebSocket->Send(UTF8Array.GetData(), UTF8Array.Num(), false);

}

void FXGWSMSeverConnection::UpdateIntervalTime()
{
	IntervalTime = 0.f;

}

bool FXGWSMSeverConnection::IsOutOfTime()
{
	if (IntervalTime > 10.f)
	{
		return true;
	}

	return false;
}

void FXGWSMSeverConnection::Tick(float InDeltaTime)
{
	IntervalTime += InDeltaTime;
}
