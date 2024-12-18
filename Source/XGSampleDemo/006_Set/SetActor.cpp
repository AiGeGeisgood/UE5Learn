#include "SetActor.h"

// Sets default values
ASetActor::ASetActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASetActor::InitSet()
{
	TSet<FString> FruitSet;


	FruitSet.Add(TEXT("Banana"));

	FruitSet.Add(TEXT("Grapefruit"));

	FruitSet.Add(TEXT("Pineapple"));

	// FruitSet == [ "Banana", "Grapefruit", "Pineapple" ]


	FruitSet.Add(TEXT("Pear"));

	FruitSet.Add(TEXT("Banana"));

	// FruitSet == [ "Banana", "Grapefruit", "Pineapple", "Pear" ]

	// Note:Only one banana entry.


	FruitSet.Emplace(TEXT("Orange"));

	// FruitSet == [ "Banana", "Grapefruit", "Pineapple", "Pear", "Orange" ]

	TSet<FString> FruitSet2;

	FruitSet2.Emplace(TEXT("Kiwi"));

	FruitSet2.Emplace(TEXT("Melon"));

	FruitSet2.Emplace(TEXT("Mango"));

	FruitSet2.Emplace(TEXT("Orange"));

	FruitSet.Append(FruitSet2);

	// FruitSet == [ "Banana", "Grapefruit", "Pineapple", "Pear", "Orange", "Kiwi", "Melon", "Mango" ]






}

void ASetActor::LoopSet()
{
	TSet<FString> FruitSet;


	FruitSet.Add(TEXT("Banana"));

	FruitSet.Add(TEXT("Grapefruit"));

	FruitSet.Add(TEXT("Pineapple"));

	for (auto& Elem : FruitSet)
	{
		Elem += TEXT("1");
		FPlatformMisc::LocalPrint(*FString::Printf(TEXT(" \"%s\"\n"), *Elem));
	}

	for (FString& Elem : FruitSet)
	{
		Elem += TEXT("2");
		FPlatformMisc::LocalPrint(*FString::Printf(TEXT(" \"%s\"\n"), *Elem));
	}


	for (auto It = FruitSet.CreateIterator(); It; ++It)
	{
		(*It) += TEXT("3");
		FPlatformMisc::LocalPrint(*FString::Printf(TEXT("(%s)\n"), *(*It)));
	}


	for (auto It = FruitSet.CreateConstIterator(); It; ++It)
	{

		FPlatformMisc::LocalPrint(*FString::Printf(TEXT("(%s)\n"), *(*It)));

	}





}

void ASetActor::QuerySet()
{
	TSet<FString> FruitSet;

	bool bHave= false;

	FruitSet.Add(TEXT("Banana"),&bHave);

	FruitSet.Add(TEXT("Grapefruit"));

	FruitSet.Add(TEXT("Pineapple"));

	FruitSet.Add(TEXT("Banana"), &bHave);

	int32 Count = FruitSet.Num();

	// Count == 3

	bool bHasBanana = FruitSet.Contains(TEXT("Banana"));

	bool bHasLemon = FruitSet.Contains(TEXT("Lemon"));

	// bHasBanana == true

	// bHasLemon == false


	FSetElementId SetElementId= FruitSet.Add(TEXT("Water"));

	FruitSet[SetElementId] +=TEXT("Modify");


	FString* PtrBanana = FruitSet.Find(TEXT("Banana"));

	FString* PtrLemon = FruitSet.Find(TEXT("Lemon"));

	// *PtrBanana == "Banana"

	//  PtrLemon == nullptr



	TArray<FString> FruitArray = FruitSet.Array();

	int32 a= 1;








}

void ASetActor::RemoveSet()
{



	TSet<FString> FruitSet;

	FruitSet.Reserve(4);


	FruitSet.Add(TEXT("Banana"));

	FruitSet.Add(TEXT("Grapefruit"));

	FruitSet.Add(TEXT("Pineapple"));

	FruitSet.Add(TEXT("GG"));

	FruitSet.Add(TEXT("HH"));

	FruitSet.Add(TEXT("JJ"));

	FruitSet.Remove(FSetElementId::FromInteger(0));

	int32 GGNum = FruitSet.Remove(TEXT("GG"));

	int32 MMNum = FruitSet.Remove(TEXT("MM"));

	TSet<FString> FruitSet1 = FruitSet;

	FruitSet1.Reset();

	TSet<FString> FruitSet2 = FruitSet;

	FruitSet2.Empty(0);

	int32 aa=1;
}

void ASetActor::SortSet()
{
	TSet<FString> FruitSet={ "Orange","Pear", "Melon", "Grapefruit" ,"Mango", "Kiwi", };



	FruitSet.Sort([](const FString& A, const FString& B) {

		return A > B; // sort by reverse-alphabetical order

		});

	// FruitSet == [ "Pear", "Orange", "Melon", "Mango", "Kiwi", "Grapefruit" ] (order is temporarily guaranteed)



	FruitSet.Sort([](const FString& A, const FString& B) {

		return A.Len() < B.Len(); // sort strings by length, shortest to longest

		});

	// FruitSet == [ "Pear", "Kiwi", "Melon", "Mango", "Orange", "Grapefruit" ] (order is temporarily guaranteed)

	//FruitSet.StableSort();

	int32 a =1;

}

void ASetActor::OpeatorSet()
{

	TSet<FString> FruitSet = { "Orange","Pear", "Melon", "Grapefruit" ,"Mango", "Kiwi", };

	TSet<FString> NewSet = FruitSet;

	NewSet.Add(TEXT("Apple"));

	NewSet.Remove(TEXT("Pear"));

	// FruitSet == [ "Pear", "Kiwi", "Melon", "Mango", "Orange", "Grapefruit" ]

	// NewSet == [ "Kiwi", "Melon", "Mango", "Orange", "Grapefruit", "Apple" ]

	int32 a = 1;
}

void ASetActor::SlackSet()
{
	TSet<FString> FruitSet = { "Orange","Pear", "Melon", "Grapefruit" ,"Mango", "Kiwi", };

	FruitSet.Reset();

	FruitSet.Reserve(10);

	for (int32 i = 0; i < 10; ++i)
	{
		FruitSet.Add(FString::Printf(TEXT("Fruit%d"), i));
	}
	// FruitSet == [ "Fruit9", "Fruit8", "Fruit7" ..."Fruit2", "Fruit1", "Fruit0" ]

	// Remove every other element from the set.

	for (int32 i = 0; i < 10; i += 2)

	{

		FruitSet.Remove(FSetElementId::FromInteger(i));

	}

	// FruitSet == ["Fruit8", <invalid>, "Fruit6", <invalid>, "Fruit4", <invalid>, "Fruit2", <invalid>, "Fruit0", <invalid> ]



	FruitSet.Shrink();

	// FruitSet == ["Fruit8", <invalid>, "Fruit6", <invalid>, "Fruit4", <invalid>, "Fruit2", <invalid>, "Fruit0" ]

	FruitSet.CompactStable();

	// FruitSet == ["Fruit8", "Fruit6", "Fruit4", "Fruit2", "Fruit0", <invalid>, <invalid>, <invalid>, <invalid> ]

	FruitSet.Shrink();

	// FruitSet == ["Fruit8", "Fruit6", "Fruit4", "Fruit2", "Fruit0" ]




}

// Called when the game starts or when spawned
void ASetActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
