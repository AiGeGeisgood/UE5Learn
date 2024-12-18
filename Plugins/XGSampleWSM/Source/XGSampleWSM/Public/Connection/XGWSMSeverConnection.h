// Copyright 2023 Xiao Gang. All Rights Reserved.

#pragma once

#include "XGWSMType.h"


class INetworkingWebSocket;

struct FXGWSMSeverConnection
{

public:

	explicit FXGWSMSeverConnection(INetworkingWebSocket* InServerWebSocket);

	FXGWSMSeverConnection(FXGWSMSeverConnection&& WebSocketConnection);

	~FXGWSMSeverConnection();

	FXGWSMSeverConnection(const FXGWSMSeverConnection&) = delete;

	FXGWSMSeverConnection& operator=(const FXGWSMSeverConnection&) = delete;

	FXGWSMSeverConnection& operator=(FXGWSMSeverConnection&&) = delete;

	void Send(const FXGWSMMessage& InMessage);

	void UpdateIntervalTime();

	bool IsOutOfTime();

	void Tick(float InDeltaTime);

	INetworkingWebSocket* ServerWebSocket = nullptr;

	FGuid ClientConnectionID;

	FGuid ServerConnectionID;

	EXGWSMServerConnectionStatus ServerConnectionStatus = EXGWSMServerConnectionStatus::None;

	float IntervalTime = 0.0f;


};