// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActorModify.h"

// Sets default values
AFloatingActorModify::AFloatingActorModify()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	//RootComponent = VisualMesh;
	///Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'
	///Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));

	




}

// Called when the game starts or when spawned
void AFloatingActorModify::BeginPlay()
{
	Super::BeginPlay();

	if (NewMesh)
	{

		VisualMesh->SetStaticMesh(NewMesh);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	
}

// Called every frame
void AFloatingActorModify::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();

	FRotator NewRotation = GetActorRotation();

	float RunningTime = GetGameTimeSinceCreation();

	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatSpeed;       //Scale our height by a factor of 20

	float DeltaRotation = DeltaTime * RotationSpeed;	//Rotate by 20 degrees per second
	NewRotation.Yaw += DeltaRotation;

	SetActorLocationAndRotation(NewLocation, NewRotation);

}


