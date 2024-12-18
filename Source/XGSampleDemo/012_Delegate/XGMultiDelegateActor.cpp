#include "XGMultiDelegateActor.h"
#include "Kismet/GameplayStatics.h"


AXGMultiExecuteActor::AXGMultiExecuteActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LocationRoot"));
	MyActorID = FGuid::NewGuid();
}

void AXGMultiExecuteActor::BeginPlay()
{


	AActor* ActorPtr = UGameplayStatics::GetActorOfClass(this, AXGMultiDelegateActor::StaticClass());

	if (AXGMultiDelegateActor* MultiDelegateActorPtr = Cast<AXGMultiDelegateActor>(ActorPtr))
	{
		int32 Health = 100;

		MyDelegateHandle = MultiDelegateActorPtr->MulityDelegate.AddUObject(this, &AXGMultiExecuteActor::Work, Health);

	}

}

void AXGMultiExecuteActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AActor* ActorPtr = UGameplayStatics::GetActorOfClass(this, AXGMultiDelegateActor::StaticClass());

	if (AXGMultiDelegateActor* MultiDelegateActorPtr = Cast<AXGMultiDelegateActor>(ActorPtr))
	{
		MultiDelegateActorPtr->MulityDelegate.Remove(MyDelegateHandle);

	}


}

void AXGMultiExecuteActor::Work(FString InFString, int32 InHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("ID:[%s],InStr:[%s],Health:[%d]"), *MyActorID.ToString(), *InFString, InHealth);

}


// Sets default values
AXGMultiDelegateActor::AXGMultiDelegateActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGMultiDelegateActor::CallSimpleMulti()
{
	//MulityDelegate.IsBound();

	//MulityDelegate.IsBoundToObject()

	MulityDelegate.Broadcast(TEXT("This is simple Multi"));

}

void AXGMultiDelegateActor::CallMoreMulti()
{
	MulityMoreDelegate.Broadcast(TEXT("This is More Multi"));
}

void AXGMultiDelegateActor::MyUFUNCTION(FString InStr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]_MyUFUNCTION"), *InStr);

}

void AXGMultiDelegateActor::PrintInStr(FString InStr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]_PrintInStr"), *InStr);

}

void MyMultiStaticMeth(FString InStr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]__MyMultiStaticMeth"), *InStr);
}


// Called when the game starts or when spawned
void AXGMultiDelegateActor::BeginPlay()
{
	Super::BeginPlay();

	MulityMoreDelegate.AddLambda([](FString InStr) {

		UE_LOG(LogTemp, Warning, TEXT("Lambda,InStr:[%s],"), *InStr);

		});

	if (!RawStruct)
	{
		RawStruct = new FXGMultiRawStruct();
	}

	MulityMoreDelegate.AddRaw(RawStruct, &FXGMultiRawStruct::PrintStr);

	TSharedRef<FXGMultiRawStruct> SmartStructPtr(new FXGMultiRawStruct());

	SmartPtr = SmartStructPtr;

	MulityMoreDelegate.AddSP(SmartStructPtr, &FXGMultiRawStruct::PrintSPStr);

	SmartStructPtr->InitDelegate(this);

	MulityMoreDelegate.AddStatic(MyMultiStaticMeth);

	TSharedRef<FXGMultiRawStructSafe,ESPMode::ThreadSafe> SmartStructPtrSafe(new FXGMultiRawStructSafe());
	SmartPtrSafe= SmartStructPtrSafe;

	MulityMoreDelegate.AddThreadSafeSP(SmartStructPtrSafe,&FXGMultiRawStructSafe::PrintSPStr);


	MulityMoreDelegate.AddUFunction(this,TEXT("MyUFUNCTION"));

	MulityMoreDelegate.AddUObject(this,&AXGMultiDelegateActor::PrintInStr);

	MulityMoreDelegate.AddWeakLambda(this,[](FString InStr){

		UE_LOG(LogTemp, Warning, TEXT("Weak-Lambda,InStr:[%s],"), *InStr);

	});

	MulityMoreDelegate = MulityDelegate;

}

void AXGMultiDelegateActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MulityDelegate.Clear();
	MulityMoreDelegate.Clear();

	if (RawStruct)
	{
		delete RawStruct;
		RawStruct = nullptr;
	}
}

// Called every frame
void AXGMultiDelegateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void FXGMultiRawStruct::InitDelegate(AXGMultiDelegateActor* InActor)
{

	InActor->MulityMoreDelegate.AddSPLambda(this, [](FString InStr) {

		UE_LOG(LogTemp, Warning, TEXT("SP- Lambda,InStr:[%s],"), *InStr);

		});


}
