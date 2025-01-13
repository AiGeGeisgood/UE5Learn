#include "XGAssertActor.h"

// Sets default values
AXGAssertActor::AXGAssertActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGAssertActor::AttackEnemey(AActor* InActor)
{
	check(InActor != nullptr);

	AXGAssertActor* MyAsserActor = CastChecked<AXGAssertActor>(InActor);

	MyAsserActor->Health -= 10;

}

void AXGAssertActor::AttackEnemeyGood(AActor* InActor)
{
	ensure(InActor != nullptr);



	AXGAssertActor* MyAsserActor = Cast<AXGAssertActor>(InActor);
	if (MyAsserActor)
	{
		MyAsserActor->Health -= 10;

		//不要这样子使用断言
		ensure(ModifyHealth());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("转型失败了!!!"));
	}



}

void AXGAssertActor::VerifyMana()
{

	verify(ModifyMana() > 0);

	//执行攻击魔法


}

void AXGAssertActor::EnsuerVersion(bool bInVersion)
{

	if (ensureMsgf(bInVersion, TEXT("当前版本不正确")))
	{

		UE_LOG(LogTemp, Warning, TEXT("版本正确"));
	}

}

void AXGAssertActor::CheckNoEnry(EXGAssertType InXGAssertType)
{
	FString Message = TEXT("None");

	switch (InXGAssertType)
	{
	case EXGAssertType::None:
		checkNoEntry();
		break;
	case EXGAssertType::Left:
	{
		Message = TEXT("Left");
		FString a = TEXT("123154");
		break;
	}
	case EXGAssertType::Mid:

		Message = TEXT("Mid");
		break;
	case EXGAssertType::Right:

		Message = TEXT("Right");
		break;
	case EXGAssertType::Max:

		checkNoEntry();
		break;
	default:
		break;
	}


}

void AXGAssertActor::CheckNoCode(bool bInAllRight)
{
	checkCode(

		if (bInAllRight)
		{
			UE_LOG(LogTemp, Error, TEXT("一切正常"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("一切不正常"));
			//check(false);

		}

			);
}

void AXGAssertActor::CheckSlowf(bool bInAllRight)
{

	checkSlow(bInAllRight);

}

void AXGAssertActor::ZeroBug(int32 InZero)
{
	int32 AB = 55 / InZero;

	int32 a = 1;
}

void AXGAssertActor::SetLocationWrong()
{
	AsyncTask(ENamedThreads::AnyThread, [this]() {

		//不要在这里打印日志
		UE_LOG(LogTemp, Warning, TEXT("危险操作,非主线程操作Actor类"));

		//不要这样写,极其错误
		while (true)
		{
			static int32 LocationAdd = 1;
			LocationAdd++;

			FPlatformProcess::Sleep(0.1);
			SetActorLocation(FVector(100.f, 100.0f, 100.f+ LocationAdd));
		}




		});


}

// Called when the game starts or when spawned
void AXGAssertActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AXGAssertActor::ModifyHealth()
{

	Health += 10;
	return true;
}

int32 AXGAssertActor::ModifyMana()
{
	Mana -= 20;
	return Mana;
}

// Called every frame
void AXGAssertActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
