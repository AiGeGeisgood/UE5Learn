// Fill out your copyright notice in the Description page of Project Settings.


#include "XGCollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


#include "XGCollidingPawnMovementComponent.h"




// Sets default values
AXGCollidingPawn::AXGCollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 根组件将成为对物理反应的球体
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// 创建并放置网格体组件，以便查看球体位置
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	// 创建可激活或停止的粒子系统
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);

	OurParticleSystem->bAutoActivate = false;

	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	///Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	if (ParticleAsset.Succeeded())

	{

		OurParticleSystem->SetTemplate(ParticleAsset.Object);

	}


	// 使用弹簧臂给予摄像机平滑自然的运动感。

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));

	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	SpringArm->TargetArmLength = 400.0f;

	SpringArm->bEnableCameraLag = true;

	SpringArm->CameraLagSpeed = 3.0f;


	// 创建摄像机并附加到弹簧臂

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	// 创建移动组件的实例，并要求其更新根。

	OurMovementComponent = CreateDefaultSubobject<UXGCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));

	OurMovementComponent->UpdatedComponent = RootComponent;






	// 控制默认玩家

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

UPawnMovementComponent* AXGCollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

// Called when the game starts or when spawned
void AXGCollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AXGCollidingPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void AXGCollidingPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void AXGCollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void AXGCollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

// Called every frame
void AXGCollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXGCollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &AXGCollidingPawn::ParticleToggle);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXGCollidingPawn::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &AXGCollidingPawn::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AXGCollidingPawn::Turn);


}

