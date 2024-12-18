// Fill out your copyright notice in the Description page of Project Settings.


#include "XGCameraDirectorModify.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

// Sets default values
AXGCameraDirectorModify::AXGCameraDirectorModify()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXGCameraDirectorModify::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> MyCamerActors;
	UGameplayStatics::GetAllActorsOfClass(this,ACameraActor::StaticClass(), MyCamerActors);

	CamerInfoList.Empty();


	check(MyCamerActors.Num()>1);

	for (int32 Index = 0; Index < MyCamerActors.Num(); Index++)
	{
		FXGToggleCamerInfo& CamerInfo =	 CamerInfoList.AddDefaulted_GetRef();

		CamerInfo.CameraOne = MyCamerActors[Index];

		CamerInfo.CameraBlendTime += Index/4.f;

	}

	CamerIndex =0;


	
}

// Called every frame
void AXGCameraDirectorModify::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	const float TimeBetweenCameraChanges = 2.0f;


	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0.0f)
	{

		TimeToNextCameraChange += TimeBetweenCameraChanges;


		++CamerIndex;

		if (CamerIndex> CamerInfoList.Num()-1)
		{
			CamerIndex=0;
		}


	
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		AActor* NextCamera = CamerInfoList[CamerIndex].CameraOne;
		float  NextBlentTime = CamerInfoList[CamerIndex].CameraBlendTime;

		if (OurPlayerController&& NextCamera)
		{
			OurPlayerController->SetViewTargetWithBlend(NextCamera, NextBlentTime);

		}


	}



}

