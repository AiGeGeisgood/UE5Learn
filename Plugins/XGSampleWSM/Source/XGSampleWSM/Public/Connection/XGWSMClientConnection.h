// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once
#include "XGWSMType.h"

class IWebSocket;

struct FXGWSMClientConnection
{

public:

	FXGWSMClientConnection(FGuid InClientConnectionID);

	FXGWSMClientConnection(TSharedPtr<IWebSocket> InClientWebSocket);

	FXGWSMClientConnection(FXGWSMClientConnection&& WebSocketConnection);

	~FXGWSMClientConnection();

	FXGWSMClientConnection(const FXGWSMClientConnection&) = delete;

	FXGWSMClientConnection& operator=(const FXGWSMClientConnection&) = delete;

	FXGWSMClientConnection& operator=(FXGWSMClientConnection&&) = delete;

	void Send(const FXGWSMMessage& InMessage);

	void UpdateIntervalTime();

	bool IsOutOfTime();

	void Tick(float InDeltaTime);

	TSharedPtr<IWebSocket> ClientWebSocket = nullptr;

	FGuid ClientConnectionID;

	FGuid ServerConnectionID;

	EXGWSMClientConnectionStatus ClientConnectionStatus = EXGWSMClientConnectionStatus::None;

	float IntervalTime = 0.0f;

	float HeartTime = 0.f;

	float HeartCheckTime = 3.f;

};