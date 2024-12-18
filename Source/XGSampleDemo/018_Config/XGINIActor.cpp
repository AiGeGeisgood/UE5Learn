#include "XGINIActor.h"

// Sets default values
AXGINIActor::AXGINIActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGINIActor::GetINIVariable()
{

	int32 XGCustomInt;
	GConfig->GetInt(TEXT("XGCustom"), TEXT("XGCustomInt"), XGCustomInt, GGameIni);
	UE_LOG(LogTemp, Warning, TEXT(" XGCustomInt: %d"), XGCustomInt);
	XGCustomInt +=10;

	GConfig->SetInt(TEXT("XGCustom"), TEXT("XGCustomInt"), XGCustomInt, GGameIni);


	GConfig->Flush(false, GGameIni);

	//FPaths
	//FFileHelper
	//FDateTime

	const FString DefaultGamePath = FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir());

	int32 XGCustomInt2;
	GConfig->GetInt(TEXT("XGCustom"), TEXT("XGCustomInt2"), XGCustomInt2, DefaultGamePath);
	UE_LOG(LogTemp, Warning, TEXT(" XGCustomInt2: %d"), XGCustomInt2);

	XGCustomInt2 += 100;

	GConfig->SetInt(TEXT("XGCustom"), TEXT("XGCustomInt2"), XGCustomInt2, DefaultGamePath);


	GConfig->Flush(false, DefaultGamePath);





}

void AXGINIActor::ModifyConfigVariable()
{

	AXGINIActor* Settings = GetMutableDefault<AXGINIActor>();

	Settings->MyConfigVariable = 420;

	FString PathToConfigFile = FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir());
	Settings->SaveConfig(CPF_Config, *PathToConfigFile);

}

// Called when the game starts or when spawned
void AXGINIActor::BeginPlay()
{
	Super::BeginPlay();


	UE_LOG(LogTemp,Warning,TEXT(" MyConfigVariable: %d"), MyConfigVariable);


}

// Called every frame
void AXGINIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
