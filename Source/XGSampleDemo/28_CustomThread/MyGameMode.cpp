// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"


MyStruct* AMyGameMode::StructPtr  = nullptr;

AMyGameMode::AMyGameMode(): TestThread(nullptr)
{
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateThread();
	if (TestThread->Init())
	{
		TestThread->CreateSafeThread();
	}
}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int32 j = GetStr()->GetA();
	UE_LOG(LogTemp, Warning, TEXT("j = %d"), j);
}

void AMyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TestThread->WaitAndCompleted();
	DestroyThread();
}

void AMyGameMode::CreateThread()
{
	if (!TestThread)
	{
		TestThread = new FXGThread();
	}
}

void AMyGameMode::DestroyThread()
{
	if (TestThread)
	{
		delete TestThread;
		TestThread = nullptr;
	}
}
