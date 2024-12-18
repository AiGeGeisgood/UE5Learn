#include "XGCountDownTimerActor.h"


#include "Components/TextRenderComponent.h"


// Sets default values
AXGCountDownTimerActor::AXGCountDownTimerActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(150.0f);
	RootComponent = CountdownText;
	CountdownTime = 3;



}

// Called when the game starts or when spawned
void AXGCountDownTimerActor::BeginPlay()
{
	Super::BeginPlay();

	UpdateTimerDisplay();

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AXGCountDownTimerActor::AdvanceTimer, 1.0f, true);

}

void AXGCountDownTimerActor::AdvanceTimer()
{
	--CountdownTime;

	UpdateTimerDisplay();

	if (CountdownTime < 1)
	{
		//倒数完成，停止运行定时器。
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		CountdownHasFinished();
	

	}



}


// Called every frame
void AXGCountDownTimerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXGCountDownTimerActor::UpdateTimerDisplay()
{
	CountdownText->SetText(FText::FromString(FString::FromInt(FMath::Max(CountdownTime, 0))));
}
void AXGCountDownTimerActor::CountdownHasFinished_Implementation()
{
	//改为特殊读出
	CountdownText->SetText(FText::FromString(TEXT("GO!")));

}


