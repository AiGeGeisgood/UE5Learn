// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTimer.h"


// Sets default values
ACustomTimer::ACustomTimer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACustomTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

