#include "MapActor.h"

// Sets default values
AMapActor::AMapActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMapActor::InitMap()
{
	TMap<int32, FString> FruitMap;

	FString& MyFruct = FruitMap.Add(5, TEXT("Banana"));
	MyFruct += TEXT("!!!");


	FruitMap.Add(2, TEXT("Grapefruit"));
	FruitMap.Add(7, TEXT("Pineapple"));
	// FruitMap == [
	// 	{ Key:5, Value:"Banana"     },
	// 	{ Key:2, Value:"Grapefruit" },
	// 	{ Key:7, Value:"Pineapple"  }
	// ]


	FruitMap.Add(2, TEXT("Pear"));
	// FruitMap == [
	// 	{ Key:5, Value:"Banana"    },
	// 	{ Key:2, Value:"Pear"      },
	// 	{ Key:7, Value:"Pineapple" }
	// ]


	FruitMap.Add(4);
	// FruitMap == [
	// 	{ Key:5, Value:"Banana"    },
	// 	{ Key:2, Value:"Pear"      },
	// 	{ Key:7, Value:"Pineapple" },
	// 	{ Key:4, Value:""          }
	// ]

	FruitMap.Emplace(3, TEXT("Orange"));

	// FruitMap == [

	// 	{ Key:5, Value:"Banana"    },

	// 	{ Key:2, Value:"Pear"      },

	// 	{ Key:7, Value:"Pineapple" },

	// 	{ Key:4, Value:""          },

	// 	{ Key:3, Value:"Orange"    }

	// ]

	TMap<int32, FString> FruitMap2;

	FruitMap2.Emplace(4, TEXT("Kiwi"));

	FruitMap2.Emplace(9, TEXT("Melon"));

	FruitMap2.Emplace(5, TEXT("Mango"));

	FruitMap.Append(FruitMap2);

	// FruitMap == [

	// 	{ Key:5, Value:"Mango"     },

	// 	{ Key:2, Value:"Pear"      },

	// 	{ Key:7, Value:"Pineapple" },

	// 	{ Key:4, Value:"Kiwi"      },

	// 	{ Key:3, Value:"Orange"    },

	// 	{ Key:9, Value:"Melon"     }

	// ]

	// FruitMap2 is now empty.

	TMultiMap<int32, FString> FruitMultiMap;

	FruitMultiMap.Add(2, TEXT("Grapefruit"));
	FruitMultiMap.Add(2, TEXT("Pineapple"));
	FruitMultiMap.Add(2, TEXT("Melon"));


	int32 a = 1;

}

void AMapActor::IterateMap()
{
	TMap<int32, FString> FruitMap;
	FruitMap.Add(1, TEXT("Grapefruit"));
	FruitMap.Add(2, TEXT("Pineapple"));
	FruitMap.Add(3, TEXT("Melon"));

	for (auto& Elem : FruitMap)
	{

		FPlatformMisc::LocalPrint(*FString::Printf(TEXT("Auto-(%d, \"%s\")\n"), Elem.Key, *Elem.Value));

		UE_LOG(LogTemp, Warning, TEXT("Auto-(%d, \"%s\")\n"), Elem.Key, *Elem.Value);

	}

	for (const TPair<int32, FString>& Element : FruitMap)
	{
		FString Message = FString::Printf(TEXT("TPair-(%d, \"%s\")\n"), Element.Key, *Element.Value);
		UE_LOG(LogTemp, Warning, TEXT("%s,%s"), *FString(__FUNCTION__), *Message);
		FPlatformMisc::LocalPrint(*Message);
	}

	for (auto It = FruitMap.CreateConstIterator(); It; ++It)

	{

		FPlatformMisc::LocalPrint(*FString::Printf(TEXT("Iterator-(%d, \"%s\")\n"), It.Key(), *It.Value()));

	}





}

void AMapActor::QueryMap()
{
	TMap<int32, FString> FruitMap;
	FruitMap.Add(5, TEXT("Grapefruit"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(9, TEXT("Melon"));

	int32 Count = FruitMap.Num();

	// Count == 3

	bool bHas7 = FruitMap.Contains(7);

	bool bHas8 = FruitMap.Contains(8);

	// bHas7 == true

	// bHas8 == false


	FString Val7 = FruitMap[7];

	// Val7 == "Pineapple"
	Val7 += TEXT("Copy");

	FString& Val7Ref = FruitMap[7];
	Val7Ref += TEXT("Ref");

	const FString& Val7ConstRef = FruitMap[7];
	//Val7ConstRef += TEXT("Ref");
	// 

	if (bHas8)
	{
		FString Val8 = FruitMap[8];
	}



	// Assert!







}

void AMapActor::FindMap()
{
	TMap<int32, FString> FruitMap;

	FString* PtrTest = FruitMap.Find(7);



	FruitMap.Add(5, TEXT("Grapefruit"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(9, TEXT("Melon"));

	FString* Ptr7 = FruitMap.Find(7);

	FString* Ptr8 = FruitMap.Find(8);

	if (FString* Ptr7Test = FruitMap.Find(7))
	{
		UE_LOG(LogTemp, Warning, TEXT("%d-%s"), 7, **Ptr7Test);

	}


	// *Ptr7 == "Pineapple"

	//  Ptr8 == nullptr



}

void AMapActor::FindAdvMap()
{
	TMap<int32, FString> FruitMap;
	FruitMap.Add(5, TEXT("Grapefruit"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(9, TEXT("Melon"));

	//可能会创建的新的元素,返回是引用
	FString& Ref7 = FruitMap.FindOrAdd(7);
	FString& Ref8 = FruitMap.FindOrAdd(8);
	Ref8 = TEXT("NewAdd8");

	FString Val7 = FruitMap.FindRef(7);
	Val7 += TEXT("998");

	//不会创建新的元素,如果没有就构建一个临时的默认值返回,返回的是值的副本
	FString Val6 = FruitMap.FindRef(6);

	int32 a = 1;

}

void AMapActor::FindKeyMap()
{
	TMap<int32, FString> FruitMap;
	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Pineapple"));
	const int32* KeyMangoPtr = FruitMap.FindKey(TEXT("Mango"));
	const int32* KeyKumquatPtr = FruitMap.FindKey(TEXT("Kumquat"));

	// *KeyMangoPtr   == 5
	//  KeyKumquatPtr == nullptr

	const int32* KeyFindTest1 = FruitMap.FindKey(TEXT("Pineapple"));
	FruitMap.Add(11, TEXT("Pineapple"));
	FruitMap.Add(12, TEXT("Pineapple"));
	FruitMap.Add(13, TEXT("Pineapple"));

	const int32* KeyFindTest2 = FruitMap.FindKey(TEXT("Pineapple"));

	int32 a = 1;
}

void AMapActor::XGGetAllKeysAndValueMap()
{
	TMap<int32, FString> FruitMap;
	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Pineapple"));



	TArray<int32>   FruitKeys;
	TArray<FString> FruitValues;


	FruitMap.GenerateKeyArray(FruitKeys);
	FruitMap.GenerateValueArray(FruitValues);


	int32 a = 1;
}

void AMapActor::XGRemoveMap()
{

	TMap<int32, FString> FruitMap;
	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Grapefruit"));

	int32 Key5toTemove = FruitMap.Remove(5);

	int32 Key8toTemove = FruitMap.Remove(8);

	if (Key5toTemove > 0)
	{

	}
	if (Key5toTemove == 0)
	{

	}



}

void AMapActor::XGRemoveCheckMap()
{
	TMap<int32, FString> FruitMap;

	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Grapefruit"));

	FString Removed5 = FruitMap.FindAndRemoveChecked(5);


	FString Removed8 = FruitMap.FindAndRemoveChecked(8);







}

void AMapActor::RemoveAndCopyValueMap()
{
	TMap<int32, FString> FruitMap;

	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Grapefruit"));

	FString Removed = TEXT("XG");

	bool bFound5 = FruitMap.RemoveAndCopyValue(5, Removed);


	bool bFound8 = FruitMap.RemoveAndCopyValue(8, Removed);





}

void AMapActor::EmptyMap()
{
	TMap<int32, FString> FruitMap;

	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Grapefruit"));

	FruitMap.Empty(0);

	FruitMap.Empty(10);

	int32 a = 1;

}

void AMapActor::ResetMap()
{
	TMap<int32, FString> FruitMap1;

	FruitMap1.Add(5, TEXT("Mango"));
	FruitMap1.Add(7, TEXT("Pineapple"));
	FruitMap1.Add(10, TEXT("Grapefruit"));


	FruitMap1.Reset();



	TMap<int32, FString> FruitMap2;

	FruitMap2.Add(5, TEXT("Mango"));
	FruitMap2.Add(7, TEXT("Pineapple"));
	FruitMap2.Add(10, TEXT("Grapefruit"));
	FruitMap2.Add(11, TEXT("Grapefruit"));
	FruitMap2.Add(12, TEXT("Grapefruit"));

	FruitMap2.Reset();

	int32 a = 1;



}

void AMapActor::ReverseMap()
{
	TMap<int32, FString> FruitMap;

	FruitMap.Reserve(3);

	FruitMap.Add(5, TEXT("Mango"));
	FruitMap.Add(7, TEXT("Pineapple"));
	FruitMap.Add(10, TEXT("Grapefruit"));

	FruitMap.Reserve(10);

	FruitMap.Reserve(2);

	int32 a = 1;

}

void AMapActor::SortMap()
{
	TMap<int32, FString> FruitMap1;

	//Lenth 9
	FruitMap1.Add(7, TEXT("Pineapple"));
	//Lenth 6
	FruitMap1.Add(5, TEXT("Mango2"));
	//Lenth 10
	FruitMap1.Add(10, TEXT("Grapefruit"));
	//Lenth 5
	FruitMap1.Add(1, TEXT("Apple"));

	 FString* MyFruit = FruitMap1.Find(10);

	 if (MyFruit)
	 {
		 (*MyFruit)+=TEXT("M1");
	 }

	FruitMap1.KeySort([](int32 A, int32 B) {

		return A > B; // sort keys in reverse

		});


	FruitMap1.ValueSort([](const FString& A, const FString& B) {

		return A.Len() < B.Len(); // sort strings by length

		});
	
	(*MyFruit)+= TEXT("M2");


	int32 a =1;





}

void AMapActor::OperateMap()
{
	TMap<int32, FString> FruitMap;

	//Lenth 9
	FruitMap.Add(7, TEXT("Pineapple"));
	//Lenth 6
	FruitMap.Add(5, TEXT("Mango2"));
	//Lenth 10
	FruitMap.Add(10, TEXT("Grapefruit"));
	//Lenth 5
	FruitMap.Add(1, TEXT("Apple"));


	TMap<int32, FString> NewMap = FruitMap;

	NewMap[5] = "Apple";

	NewMap.Remove(1);



	TMap<int32, FString> NewMap2 = MoveTemp(FruitMap);

	int32 ii =1;

	TMap<int32,AActor*> MyActorPtrs;

	MyActorPtrs.Add(1,this);
	
	MyActorPtrs.Add(2, nullptr);

	MyActorPtrs.Add(3, nullptr);

	TMap<int32, AActor*> MyAnotherActorPtrs = MyActorPtrs;

	MyAnotherActorPtrs[1]->SetActorLocation(FVector::ZeroVector);
	MyActorPtrs[1]->SetActorLocation(FVector::ZeroVector);

	bool bEqual = MyAnotherActorPtrs[1] == MyActorPtrs[1];
	int32 a =1;



}

void AMapActor::SlackMap()
{
	TMap<int32, FString> FruitMap;

	FruitMap.Reserve(10);

	FruitMap.Add(1, TEXT("A"));

	FruitMap.Add(2, TEXT("B"));
	
	FruitMap.Add(3, TEXT("C"));

	FruitMap.Add(4, TEXT("D"));

	FruitMap.Add(5, TEXT("E"));

	FruitMap.Add(6, TEXT("F"));

	FruitMap.Add(7, TEXT("G"));

	FruitMap.Add(8, TEXT("H"));

	FruitMap.Add(9, TEXT("I"));

	FruitMap.Add(10, TEXT("J"));


	FruitMap.Remove(2);

	FruitMap.Remove(4);

	FruitMap.Remove(6);

	FruitMap.Remove(8);

	FruitMap.Remove(10);

	FruitMap.Shrink();

	FruitMap.Compact();

	FruitMap.Shrink();

	int32 a = 1;


}

void AMapActor::StructMap()
{
	TMap<FXGMapInfo,int32> MyNPCs;

	MyNPCs.Add(FXGMapInfo(TEXT("Boss")),1);

	MyNPCs.Add(FXGMapInfo(TEXT("Player")), 2);

	MyNPCs.Add(FXGMapInfo(TEXT("Tree")), 1);

	MyNPCs.Add(FXGMapInfo(TEXT("player")), 3);

	//保证有10个敌人
	MyNPCs.FindOrAdd(FXGMapInfo(TEXT("NormalEnemy")),10);

	//保证有至少有个4个玩家
	int32& PlayerNum =MyNPCs.FindOrAdd(FXGMapInfo(TEXT("Player")));
	PlayerNum+=4;

	int32 a =1;

	

}

void AMapActor::StructKeyFunMap()
{
	//FMyStruct MyStruct = 10.f;

	TMap<FMyStruct,int32,FDefaultSetAllocator,TMyStructMapKeyFuncs<int32>> MyMapToInt32;

	MyMapToInt32.Add(FMyStruct(3.14f), 5);
	MyMapToInt32.Add(FMyStruct(1.23f), 2);

	TMap<FMyStruct, int32, FDefaultSetAllocator, TMyStructMapKeyFuncs<float>> MyMapToFloat;

	MyMapToFloat.Add(FMyStruct(3.14f), 50.f);
	MyMapToFloat.Add(FMyStruct(1.23f), 20.f);

	int32 a=1;


}

void AMapActor::StructSize()
{
	TMap<int32,FString> MyMap;

	MyMap.Reserve(4);

	uint32  MapSize2  = MyMap.GetAllocatedSize();
	MyMap.Add(1,TEXT("A"));
	MyMap.Add(2, TEXT("A"));
	MyMap.Add(3, TEXT("A"));

	uint32  MapSize3 = MyMap.GetAllocatedSize();

	MyMap.Add(4, TEXT("A"));
	MyMap.Add(5, TEXT("A"));
	MyMap.Add(6, TEXT("A"));
	uint32  MapSize4 = MyMap.GetAllocatedSize();
	//MyMap.CountBytes();
	//MyMap.Dump();

	int32 a =-1;
}

// Called when the game starts or when spawned
void AMapActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
