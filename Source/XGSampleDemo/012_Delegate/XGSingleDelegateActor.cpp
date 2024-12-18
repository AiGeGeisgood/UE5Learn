#include "XGSingleDelegateActor.h"
#include "Kismet/GameplayStatics.h"

AXGLocationActor::AXGLocationActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LocationRoot"));

}

void AXGLocationActor::BeginPlay()
{




}

void AXGLocationActor::PrintLocation()
{
	FVector MyLocation = GetActorLocation();

	UE_LOG(LogTemp,Warning,TEXT("[%s]__MyLocation:[%s]"),*FString(__FUNCTION__), *MyLocation.ToString());
}

FVector AXGLocationActor::GetLocation(FString InStr)
{

	FVector MyLocation = GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("[%s]__MyLocation:[%s]"), *InStr, *MyLocation.ToString());

	return MyLocation;
}

FVector AXGLocationActor::GetLocationWithPayload(FString InStr, int32 Health, int32 Money)
{
	FVector MyLocation = GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("[%s]__MyLocation:[%s]"), *InStr, *MyLocation.ToString());

	UE_LOG(LogTemp, Warning, TEXT("[%s]--Health:[%d]--Money:[%d]"), *InStr, Health, Money);

	UE_LOG(LogTemp, Warning, TEXT("[%s]--Mana:[%d]"), *InStr, Mana);

	return FVector();

}




// Sets default values
AXGSingleDelegateActor::AXGSingleDelegateActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGSingleDelegateActor::CallLocationActorPrint()
{



	if (SingDelegatePrintLocation.IsBound())
	{
		SingDelegatePrintLocation.Execute();
		//SingDelegatePrintLocation.Unbind();
	}

	SingDelegatePrintLocation.ExecuteIfBound();

}

void AXGSingleDelegateActor::CallLocationActorGet()
{
	 FVector MyReceivedLocation = SingDelegateGetLocation.Execute(TEXT("My Single Delegate Actor"));

	 UE_LOG(LogTemp, Warning, TEXT("[MyReceived]__MyLocation:[%s]"), *MyReceivedLocation.ToString());

}

void AXGSingleDelegateActor::CallLocationActorGetWithPayload()
{
	FVector MyReceivedLocation = SingDelegateGetLocationWithPayload.Execute(TEXT("My Single Delegate Actor"));

	UE_LOG(LogTemp, Warning, TEXT("[MyReceived]__MyLocation:[%s]"), *MyReceivedLocation.ToString());

}

void AXGSingleDelegateActor::CallMySelfLambda()
{
	SingDelegateLambda.ExecuteIfBound();
	SingDelegateLambdaLocation.Execute(TEXT("This is Lambda"));

}

void AXGSingleDelegateActor::CallMySelfRaw()
{

	SingDelegateRaw.ExecuteIfBound();


}

void AXGSingleDelegateActor::CallMySelfSmartPointer()
{
	SingDelegateSmartPointer.ExecuteIfBound();
}

void AXGSingleDelegateActor::CallMySelfSmartPointerLambda()
{
	SingDelegateSmartPointerLambda.ExecuteIfBound();
}

void AXGSingleDelegateActor::CallMyStaticMethod()
{
	SingDelegateStaticMethod.ExecuteIfBound();

}

void AXGSingleDelegateActor::CallMySelfSmartPointerSafe()
{

	SingDelegateSmartPointerSafe.ExecuteIfBound();
}

void AXGSingleDelegateActor::CallMySelfUFuncion()
{
	SingDelegateUFunction.ExecuteIfBound();

}

void AXGSingleDelegateActor::CallMySelfUObject()
{
	SingDelegateUObject.ExecuteIfBound();
}

void AXGSingleDelegateActor::CallMySelfWeakLambda()
{
	SingDelegateWeakLambda.ExecuteIfBound();
}

void AXGSingleDelegateActor::CallMySelfReplace()
{
	SingDelegateReplace.ExecuteIfBound();
}
void AXGSingleDelegateActor::CallReplaceMyDeleage()
{
	auto ReplaceDelegage = FXGSingDelegatePrintLocation::CreateLambda([]() {


		UE_LOG(LogTemp, Warning, TEXT("[%s]__Replace!!!!"), *FString(__FUNCTION__));

		});
	SingDelegateReplace = ReplaceDelegage;


}

void AXGSingleDelegateActor::CallReplaceMyDeleage2(FXGSingDelegatePrintLocation InReplaceDelegate)
{
	SingDelegateReplace = InReplaceDelegate;
}


void MyStaticMeth()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]__SM_MyStaticMeth"), *FString(__FUNCTION__));
}


// Called when the game starts or when spawned
void AXGSingleDelegateActor::BeginPlay()
{
	Super::BeginPlay();


	SingDelegateLambda.BindLambda([](){
	
		UE_LOG(LogTemp, Warning, TEXT("[%s]__SingDelegate_Exceute"), *FString(__FUNCTION__));

	});

	SingDelegateLambdaLocation.BindLambda([](FString InStr)->FVector{

		FVector MyLocation = FVector::ZeroVector;

		UE_LOG(LogTemp, Warning, TEXT("[%s]__MyLocation:[%s]"), *InStr, *MyLocation.ToString());

		return MyLocation;
	});

	//FXGSingeRawStruct RawStructPtr
	//不是很合适的写法
	FXGSingeRawStruct* RawStructPtr= new FXGSingeRawStruct();

	int32  PayloadMoney = 32;
	SingDelegateRaw.BindRaw(RawStructPtr,&FXGSingeRawStruct::RawLocation, PayloadMoney);


	TSharedRef<FXGSingeRawStruct> StructPtr(new FXGSingeRawStruct());
	SmartStructPtr = StructPtr;

	SingDelegateSmartPointer.BindSP(StructPtr,&FXGSingeRawStruct::SmartLocation);


	SingDelegateSmartPointerLambda.BindSPLambda(StructPtr,[](){
		UE_LOG(LogTemp, Warning, TEXT("[%s]__SM_Lambda"), *FString(__FUNCTION__));
	});


	SingDelegateStaticMethod.BindStatic(MyStaticMeth);

	//注意这个智能指针过了作用域会析构,其他地方没有智能指针对这个对象实例进行保护了!
	TSharedRef<FXGSingeRawStruct, ESPMode::ThreadSafe> StructPtrSafe(new FXGSingeRawStruct());
	SingDelegateSmartPointerSafe.BindThreadSafeSP(StructPtrSafe, &FXGSingeRawStruct::SmartLocation);



	SingDelegateUFunction.BindUFunction(this,TEXT("MyFunction"));

	SingDelegateUObject.BindUObject(this,&AXGSingleDelegateActor::MyFunction);

	SingDelegateWeakLambda.BindWeakLambda(this,[](){
	
		UE_LOG(LogTemp, Warning, TEXT("[%s]__Bind from WeakLambda"), *FString(__FUNCTION__));
	});


	auto MyDelegage=  FXGSingDelegatePrintLocation::CreateUObject(this, &AXGSingleDelegateActor::MyFunction);

	SingDelegateReplace = MyDelegage;

}

void AXGSingleDelegateActor::MyFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]__Bind from UFunction"), *FString(__FUNCTION__));

}

// Called every frame
void AXGSingleDelegateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

