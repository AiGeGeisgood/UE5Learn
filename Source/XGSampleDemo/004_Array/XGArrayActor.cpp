#include "XGArrayActor.h"

// Sets default values
AXGArrayActor::AXGArrayActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGArrayActor::InitIntArray()
{
	TArray<int32> IntArray1;
	TArray<int32> IntArray2;

	int32 ArrayNum = IntArray1.Num();
	int32 ArraySize = IntArray1.GetAllocatedSize();

	IntArray1.Init(10, 5);
	// IntArray == [10,10,10,10,10]
	IntArray2 = { 1,2,3,4,5,6 };

	ArrayNum = IntArray1.Num();
	ArraySize = IntArray1.GetAllocatedSize();


}

void AXGArrayActor::AddStrArray()
{
	TArray<FString> StrArr;

	StrArr.Add(TEXT("Hello"));

	StrArr.Emplace(TEXT("World"));

	StrArr.Add(TEXT("My"));

	StrArr.Add(TEXT("City"));

	StrArr.Add(TEXT("ChengDu"));

	StrArr.Add(TEXT("Well"));


}

void AXGArrayActor::AddStructArray()
{
	StructArray.Add(150);

}

void AXGArrayActor::EmplaceStructArray()
{
	StructArray.Emplace(200);

}

TArray<FString> AXGArrayActor::AppendStrArray()
{
	FString Arr[] = { TEXT("of"), TEXT("Tomorrow") };

	TArray<FString> StrArr;

	StrArr.Add(TEXT("Hello"));

	StrArr.Emplace(TEXT("World"));

	StrArr.Append(Arr, UE_ARRAY_COUNT(Arr));

	// StrArr == ["Hello","World","of","Tomorrow"]

	return StrArr;

}

void AXGArrayActor::AddUniqueString()
{
	TArray<FString> StrArr = AppendStrArray();

	StrArr.AddUnique(TEXT("!"));

	// StrArr == ["Hello","World","of","Tomorrow","!"]

	StrArr.AddUnique(TEXT("!"));

	// StrArr is unchanged as "!" is already an element





}

void AXGArrayActor::InsertString()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	StrArr.Insert(TEXT("Brave"), 1);

	// StrArr == ["Hello","Brave","World","of","Tomorrow","!"]


}

void AXGArrayActor::XGSetStringNum()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	StrArr.SetNum(10);

	StrArr.SetNum(5);
}

void AXGArrayActor::AddUniqueStruct()
{
	TArray<FXGEqualStructInfo> EqualStructArray;

	EqualStructArray.AddUnique(0);

	EqualStructArray.AddUnique(1);

	EqualStructArray.AddUnique(1);

	EqualStructArray.AddUnique(2);

	EqualStructArray.AddUnique(3);
}
#pragma region ArrayRegion
void AXGArrayActor::XGLoopArray1()
{
	FString JoinedStr1;
	FString JoinedStr2;
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };


	for (const auto& Str : StrArr)
	{

		JoinedStr1 += Str;

		JoinedStr1 += TEXT(" ");

	}


	for (auto Str : StrArr)
	{

		JoinedStr2 += Str;

		JoinedStr2 += TEXT(" ");

	}

	// JoinedStr == "Hello Brave World of Tomorrow !"



}

void AXGArrayActor::XGLoopArray2()
{
	FString JoinedStr;
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	for (int32 Index = 0; Index != StrArr.Num(); ++Index)
	{

		JoinedStr += StrArr[Index];

		JoinedStr += TEXT(" ");

	}



}

void AXGArrayActor::XGLoopArray3()
{
	FString JoinedStr;
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	for (auto It = StrArr.CreateConstIterator(); It; ++It)
	{

		JoinedStr += *It;

		JoinedStr += TEXT(" ");

	}



}

void AXGArrayActor::XGLoopArray_Error()
{
	FString JoinedStr;
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	for (int32 Index = 0; Index != StrArr.Num(); ++Index)
	{
		if (TEXT("of") == StrArr[Index])
		{

			StrArr.RemoveAt(Index);
		}
		else
		{
			JoinedStr += StrArr[Index];
			JoinedStr += TEXT(" ");
		}



	}

}

void AXGArrayActor::XGLoopArray_Right()
{
	FString JoinedStr;
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	for (int32 Index = StrArr.Num() - 1; Index >= 0; --Index)
	{
		if (TEXT("of") == StrArr[Index])
		{

			StrArr.RemoveAt(Index);
		}
		else
		{
			JoinedStr = StrArr[Index] + TEXT(" ") + JoinedStr;


		}


	}


}

void AXGArrayActor::XGLoopArray_Right_2()
{
	TArray<FString> StrArr = { "Hello", "My","World", "of", "Tomorrow", "!" };

	TArray<int32> RemovedIndexArray;


	for (int32 Index = StrArr.Num() - 1; Index >= 0; --Index)
	{
		if (TEXT("of") == StrArr[Index])
		{

			RemovedIndexArray.Add(Index);
		}

		if (TEXT("My") == StrArr[Index])
		{

			RemovedIndexArray.Add(Index);
		}
	}

	for (int32 RemovedLoopIndex = 0; RemovedLoopIndex != RemovedIndexArray.Num(); ++RemovedLoopIndex)
	{
		StrArr.RemoveAt(RemovedIndexArray[RemovedLoopIndex]);
	}


	//注意一定要分帧执行,
	//不要在WebSocket通信过程去创建新WebSocket

}







#pragma endregion ArrayRegion

#pragma region Sort


void AXGArrayActor::XGSortArray_Sort()
{
	TArray<FString> StrArr = { "aa","ab","ABC","BB","VE","AA","CCCCC","AD","AB","DDDDDA","AA" };

	StrArr.Sort();

}

void AXGArrayActor::XGSortArray_HeapSort()
{

	TArray<FString> StrArr = { "aa","ab","ABC","BB","VE","AA","CCCCC","AD","AB","DDDDDA","AA" };

	StrArr.HeapSort();
}

void AXGArrayActor::XGSortArray_StableSort()
{
	TArray<FString> StrArr = { "aa","ab","ABC","BB","VE","AA","CCCCC","AD","AB","DDDDDA","AA" };

	StrArr.StableSort();
}

void AXGArrayActor::TestLambda()
{
	int32 OriginNum = 100;

	auto LambdaMethod = [OriginNum](int32 InNum, int32 InAddNum)->int32 {

		int32 Max = InNum + InAddNum;

		Max += OriginNum;

		return Max;
		};

	int32 CalculateNum1 = LambdaMethod(10, 2);

	int32 CalculateNum2 = CalculateMethod(10, 2);
}

int32 AXGArrayActor::CalculateMethod(int32 InNum, int32 InAddNum)
{
	int32 Max = InNum + InAddNum;

	Max += TempNum;

	return Max;
}

void AXGArrayActor::XGSortArray_Sort_2()
{
	TArray<FString> StrArr = { "CCCCC","BB","aa","ab","ABC","VE","AA","AD","AB","DDDDDA","AA" };

	StrArr.Sort([](const FString& A, const FString& B) {

		return A.Len() < B.Len();

		});




}

void AXGArrayActor::XGSortArray_HeapSort_2()
{

	TArray<FString> StrArr = { "CCCCC","BB","aa","ab","ABC","VE","AA","AD","AB","DDDDDA","AA" };

	StrArr.HeapSort([](const FString& A, const FString& B) {

		return A.Len() < B.Len();

		});
}

void AXGArrayActor::XGSortArray_StableSort_2()
{
	TArray<FString> StrArr = { "CCCCC","BB","aa","ab","ABC","VE","AA","AD","AB","DDDDDA","AA" };

	StrArr.StableSort([](const FString& A, const FString& B) {

		return A.Len() < B.Len();

		});

}

void AXGArrayActor::XGSortStructArray_StableSort()
{

	TArray<FXGSortStructInfo> MyTeams1;



	MyTeams1.Add({ 99,2 });

	MyTeams1.Add({ 1,10 });

	MyTeams1.Add({ 66,3 });

	MyTeams1.Add({ 88,60 });

	TArray<FXGSortStructInfo> MyTeams2;

	MyTeams2.Add({ 99,2 });

	MyTeams2.Add({ 1,10 });

	MyTeams2.Add({ 66,3 });

	MyTeams2.Add({ 88,60 });


	MyTeams1.StableSort();

	MyTeams2.StableSort([](const FXGSortStructInfo& A, const FXGSortStructInfo& B)->bool {


		return A.Money < B.Money ? true : false;

		});



}

void AXGArrayActor::XGFindArray()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	int32 ArraySize = StrArr.Num();

	FString* MyFirsetStr = StrArr.GetData();

	FString* StrPtr = StrArr.GetData();

	bool bP0 = StrPtr[0] == "Hello";

	bool bP1 = StrPtr[1] == "World";

	bool bP2 = StrPtr[2] == "of";

	bool bP3 = StrPtr[3] == "Tomorrow";

	FString MyTemp = *(StrPtr + 4);

	FString MyTemp2 = *StrPtr + TEXT("4");

	bool bP4 = *(StrPtr + 4) == "of";

	StrPtr[5];

	//下面这个越界
	StrArr[5];


}

void AXGArrayActor::XGFindArray_Change()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };
	TArray<FString> StrArr2 = { "2","3","4","5" };

	StrArr.Append(StrArr2);
	StrArr.Append(StrArr2);
	StrArr.Append(StrArr2);
	StrArr.Append(StrArr2);

	FString* StrPtr = StrArr.GetData();

	bool bP0 = StrPtr[0] == "Hello";

	bool bP1 = StrPtr[1] == "World";

	bool bP2 = StrPtr[2] == "of";

	bool bP3 = StrPtr[3] == "Tomorrow";




	bool bPZ0 = StrArr[0] == "Hello";

	bool bPZ1 = StrArr[1] == "World";

	bool bPZ2 = StrArr[2] == "of";

	bool bPZ3 = StrArr[3] == "Tomorrow";


}

void AXGArrayActor::XGFindArray_Const()
{

	const TArray< FString> StrArr = { TEXT("Hello"), TEXT("World"), TEXT("of"), TEXT("Tomorrow"), TEXT("!") };

	const FString* StrPtr = StrArr.GetData();

	//*StrPtr =*StrPtr+TEXT("Test");

}

void AXGArrayActor::XGFindArray_ElementSize()
{

	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	uint32 ElementSize1 = StrArr.GetTypeSize();

	uint32 ElementSize2 = sizeof(FString);

	TArray<int32> IntArr = { 2, 3, 4, 5,6 };

	uint32 ElementSize3 = IntArr.GetTypeSize();

	TArray<uint8> Uint8Arr = { 2, 3, 4, 5,6 };

	uint32 ElementSize4 = Uint8Arr.GetTypeSize();
}

void AXGArrayActor::XGGetAllocatedSize()
{
	//	一个int32 是4个字节
	TArray<int32> MyInt ;

	MyInt.Add(1);

	// 16
	int32 AllocatedSize_1 = MyInt.GetAllocatedSize();

	MyInt.Empty(3);

	// 12
	int32 AllocatedSize_2 = MyInt.GetAllocatedSize();

	//一个 指针在64位操作系统上是8个字节
	TArray<AActor*> MyPointers;

	MyPointers.Add(this);

	// 32
	int32 AllocatedSize_3 = MyPointers.GetAllocatedSize();


}

void AXGArrayActor::XGIndexArray()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	FString Elem1 = StrArr[1];

	Elem1 += TEXT("10");


	FString& Elem1_X = StrArr[1];

	Elem1_X += TEXT("10");


}

void AXGArrayActor::XGIndexArray_IsValid()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	bool bValidM1 = StrArr.IsValidIndex(-1);

	bool bValid0 = StrArr.IsValidIndex(0);

	bool bValid1 = StrArr.IsValidIndex(1);

	bool bValid2 = StrArr.IsValidIndex(2);

	bool bValid3 = StrArr.IsValidIndex(3);

	bool bValid4 = StrArr.IsValidIndex(4);

	bool bValid5 = StrArr.IsValidIndex(5);

	bool bValid6 = StrArr.IsValidIndex(6);




}

void AXGArrayActor::XGIndexArray_Upper()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	StrArr[1] = StrArr[1].ToUpper();
}

void AXGArrayActor::XGIndexArray_Latest()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };


	FString ElemEnd = StrArr.Last();

	FString ElemEnd0 = StrArr.Last(0);

	FString ElemEnd1 = StrArr.Last(1);

	FString ElemTop = StrArr.Top();

	int32 a = 1;
	// ElemEnd  == "Tomorrow"

	// ElemEnd0 == "Tomorrow"

	// ElemEnd1 == "World"

	// ElemTop  == "Tomorrow"


}

void AXGArrayActor::XGContainArray()
{
	TArray<FString> StrArr = { "12347756","33","44","Hello", "World", "of", "Tomorrow", "!" };

	bool bHello = StrArr.Contains(TEXT("Hello"));

	bool bGoodbye = StrArr.Contains(TEXT("Goodbye"));

	// bHello   == true

	// bGoodbye == false

	int32 Find5Character = 0;

	bool bLen5 = StrArr.ContainsByPredicate([&Find5Character](const FString& Str) {

		++Find5Character;

		UE_LOG(LogTemp, Warning, TEXT("bLen5, [%d]"), Find5Character);

		return Str.Len() == 5;

		});

	int32 Find6Character = 0;

	bool bLen6 = StrArr.ContainsByPredicate([&Find6Character](const FString& Str) {

		++Find6Character;

		UE_LOG(LogTemp, Warning, TEXT("bLen6, [%d]"), Find6Character);


		return Str.Len() == 6;

		});

	// bLen5 == true

	// bLen6 == false


}

void AXGArrayActor::XGFindElementArray()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "Hello","!" };

	int32 Index = -1;
	bool bIndex = StrArr.Find(TEXT("Hello"), Index);


	int32 LasetIndex = -1;
	bool bIndexFromLast = StrArr.FindLast(TEXT("Hello"), LasetIndex);


	int32 ErrIndex = -1;
	bool bErrIndex = StrArr.Find(TEXT("Hello22"), ErrIndex);



	int32 Index2 = StrArr.Find(TEXT("Hello"));

	int32 IndexLast2 = StrArr.FindLast(TEXT("Hello"));

	int32 IndexNone = StrArr.Find(TEXT("None"));

	bool bIndexNone = INDEX_NONE == IndexNone;
	// IndexNone  == INDEX_NONE


}



#pragma endregion Sort


#pragma region Find




#pragma endregion Find

#pragma region Contain


#pragma endregion Contain

void AXGArrayActor::XGFindElementByKey()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!" };

	//0
	int32 Index = StrArr.IndexOfByKey(TEXT("Hello"));

	//1
	int32 IndexP = StrArr.IndexOfByPredicate([](const FString& Str) {

		return Str.Contains(TEXT("r"));

		});


	TArray<FXGFindStructInfo> StructArr;

	StructArr.Add(FXGFindStructInfo(1, 20));
	StructArr.Add(FXGFindStructInfo(2, 30));
	StructArr.Add(FXGFindStructInfo(4, 50));

	bool bFind = StructArr[1] == 2;
	int32 IndexStuctP = StructArr.IndexOfByKey(4);




}

void AXGArrayActor::XGFindElementRetPtr()
{
	TArray<FString> StrArr = { "Hello", "World", "of", "Tomorrow", "!","123456" };

	auto* OfPtr = StrArr.FindByKey(TEXT("of"));

	auto* ThePtr = StrArr.FindByKey(TEXT("the"));

	FString* RealOfPtr = StrArr.FindByKey(TEXT("of"));

	FString* RealThePtr = StrArr.FindByKey(TEXT("the"));

	if (RealOfPtr)
	{
		(*RealOfPtr) += TEXT("Modify");
	}



	TArray<AActor*> MyActors = { this,nullptr,nullptr };

	auto* ThisActor1 = MyActors.FindByKey(this);
	AActor** ThisActor2 = MyActors.FindByKey(this);

	if (AActor** ThisActor3 = MyActors.FindByKey(this))
	{
		(*ThisActor3)->SetActorLocation(FVector::ZeroVector);
	}

	auto* Len5Ptr = StrArr.FindByPredicate([](const FString& Str) {

		return Str.Len() == 5;

		});

	auto* Len6Ptr = StrArr.FindByPredicate([](const FString& Str) {

		return Str.Len() == 6;

		});

	// Len5Ptr == &StrArr[2]

	// Len6Ptr == nullptr

	auto Filter = StrArr.FilterByPredicate([](const FString& Str) {

		return !Str.IsEmpty() && Str[0] < TEXT('M');

		});



}

void AXGArrayActor::XGRemoveElement()
{
	TArray<int32> ValArr;

	int32 Temp[] = { 10, 20, 30, 5, 10, 15, 20, 25, 30 };

	//ValArr.Append(Temp, UE_ARRAY_COUNT(Temp));

	ValArr.Append(Temp, 9);

	// ValArr == [10,20,30,5,10,15,20,25,30]

	ValArr.Remove(20);

	// ValArr == [10,30,5,10,15,25,30]

	ValArr.RemoveSingle(30);

	// ValArr == [10,5,10,15,25,30]

	ValArr.RemoveAt(2); // Removes the element at index 2

	// ValArr == [10,5,15,25,30]

	ValArr.Shrink();

	ValArr.RemoveAt(99); // This will cause a runtime error as

	// there is no element at index 99




}

void AXGArrayActor::XGRemoveMultiElement()
{
	TArray<int32> ValArr;

	int32 Temp[] = { 1, 2, 3, 4, 1, 2, 3, 4, 5 };

	ValArr.Append(Temp, 9);


	ValArr.RemoveAll([](int32 Val) {

		return Val % 3 == 0;

		});

	ValArr.Empty();
	ValArr.Append(Temp, 9);
	ValArr.RemoveSwap(2);

	ValArr.Empty();
	ValArr.Append(Temp, 9);
	ValArr.RemoveAtSwap(0);

	ValArr.Empty(10);
	ValArr.Append(Temp, 9);
	ValArr.RemoveAllSwap([](int32 Val) {

		return Val % 3 == 0;

		});


}

void AXGArrayActor::XGOperateArray()
{
	TArray<int32> ValArr3;

	ValArr3.Add(1);

	ValArr3.Add(2);

	ValArr3.Add(3);

	auto ValArr4 = ValArr3;

	TArray<int32> ValArr5 = ValArr3;

	// ValArr4 == [1,2,3];

	ValArr4[0] = 5;

	// ValArr3 == [1,2,3];

	// ValArr4 == [5,2,3];



	TArray<int32> ValArr6;
	ValArr6 += ValArr4;

	ValArr6.Append(ValArr3);

	TArray<int32> ValArr7 = { 1,1,1 };

	ValArr7 = MoveTemp(ValArr6);


}

void AXGArrayActor::XGOperateStrArray()
{
	TArray<FString> FlavorArr1;

	FlavorArr1.Emplace(TEXT("Chocolate"));

	FlavorArr1.Emplace(TEXT("Vanilla"));

	// FlavorArr1 == ["Chocolate","Vanilla"]

	auto FlavorArr2 = FlavorArr1;

	// FlavorArr2 == ["Chocolate","Vanilla"]



	bool bComparison1 = FlavorArr1 == FlavorArr2;

	// bComparison1 == true



	for (auto& Str : FlavorArr2)

	{

		Str = Str.ToUpper();

	}

	// FlavorArr2 == ["CHOCOLATE","VANILLA"]



	bool bComparison2 = FlavorArr1 == FlavorArr2;


	// bComparison2 == true, because FString comparison ignores case



	Exchange(FlavorArr2[0], FlavorArr2[1]);

	// FlavorArr2 == ["VANILLA","CHOCOLATE"]



	bool bComparison3 = FlavorArr1 == FlavorArr2;

	// bComparison3 == false, because the order has changed



}

void AXGArrayActor::XGHeapArray()
{
	TArray<int32> HeapArr;

	for (int32 Val = 10; Val != 0; --Val)

	{

		HeapArr.Add(Val);

	}

	// HeapArr == [10,9,8,7,6,5,4,3,2,1]

	HeapArr.Heapify();

	// HeapArr == [1,2,4,3,6,5,8,10,7,9]


	HeapArr.HeapPush(4);

	// HeapArr == [1,2,4,3,4,5,8,10,7,9,6]

	int32 TopNode;

	HeapArr.HeapPop(TopNode);

	// TopNode == 1

	// HeapArr == [2,3,4,6,4,5,8,10,7,9]

	int32 Top = HeapArr.HeapTop();

	// Top == 2





}

void AXGArrayActor::XGSlackArray()
{
	TArray<int32> SlackArray;

	int32 SlackNum = SlackArray.GetSlack();

	int32 Num = SlackArray.Num();

	int32 Max = SlackArray.Max();


	SlackArray.Add(1);


	SlackNum = SlackArray.GetSlack();

	Num = SlackArray.Num();

	Max = SlackArray.Max();



	SlackArray.Add(2);

	SlackArray.Add(3);

	SlackArray.Add(4);

	SlackArray.Add(5);

	// SlackArray.GetSlack() == 17

	// SlackArray.Num()      == 5

	// SlackArray.Max()      == 22

	SlackNum = SlackArray.GetSlack();

	Num = SlackArray.Num();

	Max = SlackArray.Max();






}

void AXGArrayActor::XGEmptyArray()
{
	TArray<int32> SlackArray;
	SlackArray.Add(10);

	SlackArray.Add(11);
	SlackArray.Empty();

	// SlackArray.GetSlack() == 0

	// SlackArray.Num()      == 0

	// SlackArray.Max()      == 0

	SlackArray.Empty(3);

	// SlackArray.GetSlack() == 3

	// SlackArray.Num()      == 0

	// SlackArray.Max()      == 3

	SlackArray.Add(1);

	SlackArray.Add(2);

	SlackArray.Add(3);
	//最大容量变成3->   4+1+16
	//		Grow = SIZE_T(NumElements) + 3 * SIZE_T(NumElements) / 8 + ConstantGrow;
	SlackArray.Add(4);

	// SlackArray.GetSlack() == 0

	// SlackArray.Num()      == 3

	// SlackArray.Max()      == 3


}

void AXGArrayActor::XGResetArray()
{
	TArray<int32> SlackArray;

	SlackArray.Add(1);

	SlackArray.Add(2);

	SlackArray.Add(3);

	SlackArray.Reset(0);

	// SlackArray.GetSlack() == 3

	// SlackArray.Num()      == 0

	// SlackArray.Max()      == 3

	SlackArray.Reset(10);


	// SlackArray.GetSlack() == 10

	// SlackArray.Num()      == 0

	// SlackArray.Max()      == 10

	SlackArray.Shrink();



}

void AXGArrayActor::XGAddCapArray()
{

	TArray<int32> SlackArray;

	SlackArray.Add(1);

	SlackArray.Add(2);

	SlackArray.Add(3);

	SlackArray.Add(3);

	SlackArray.Add(3);


}

void AXGArrayActor::XGOriginArray()
{
	int32 SrcInts[] = { 2, 3, 5, 7 };

	TArray<int32> UninitInts;

	UninitInts.AddUninitialized(4);

	FMemory::Memcpy(UninitInts.GetData(), SrcInts, 4 * sizeof(int32));

	// UninitInts == [2,3,5,7]


	TArray<FXGAddStructInfo> UninitStructs;

	UninitStructs.AddUninitialized(4);



}

void AXGArrayActor::XGOriginArray2()
{
	TArray<FString> UninitStrs;

	UninitStrs.Emplace(TEXT("A"));

	UninitStrs.Emplace(TEXT("D"));

	UninitStrs.InsertUninitialized(1, 2);

	new ((void*)(UninitStrs.GetData() + 1)) FString(TEXT("B"));

	new ((void*)(UninitStrs.GetData() + 2)) FString(TEXT("C"));

	// UninitStrs == ["A","B","C","D"]

	TArray<FXGAddStructInfo> UninitStructs;

	UninitStructs.Emplace(20);

	UninitStructs.Emplace(50);

	UninitStructs.InsertUninitialized(1, 2);

	new ((void*)(UninitStructs.GetData() + 1)) FXGAddStructInfo(99);

	new ((void*)(UninitStructs.GetData() + 2)) FXGAddStructInfo(88);

	UninitStructs.Empty();



	char* buffer = new char[4];

	FXGAddStructInfo* Struct = new (buffer)FXGAddStructInfo;

	delete []buffer;

	//delete Struct;


}

void AXGArrayActor::XGZeroArray()
{

	TArray<FXGAddStructInfo> UninitStructs;

	UninitStructs.AddZeroed();

	UninitStructs.Empty();



}

void AXGArrayActor::XGZeroAndUninitArray()
{
	struct S

	{

		S(int32 InInt, void* InPtr, float InFlt)

			:Int(InInt)

			, Ptr(InPtr)

			, Flt(InFlt)

		{

		}

		int32 Int;

		void* Ptr;

		float Flt;

	};

	TArray<S> SArr;

	SArr.AddZeroed();

	// SArr == [{ Int:0, Ptr: nullptr, Flt:0.0f }]

	SArr.SetNumUninitialized(3);

	new ((void*)(SArr.GetData() + 1)) S(5, (void*)0x12345678, 3.14);

	new ((void*)(SArr.GetData() + 2)) S(2, (void*)0x87654321, 2.72);

	// SArr == [

	//   { Int:0, Ptr: nullptr,    Flt:0.0f  },

	//   { Int:5, Ptr:0x12345678, Flt:3.14f },

	//   { Int:2, Ptr:0x87654321, Flt:2.72f }

	// ]



	SArr.SetNumZeroed(5);

	// SArr == [

	//   { Int:0, Ptr: nullptr,    Flt:0.0f  },

	//   { Int:5, Ptr:0x12345678, Flt:3.14f },

	//   { Int:2, Ptr:0x87654321, Flt:2.72f },

	//   { Int:0, Ptr: nullptr,    Flt:0.0f  },

	//   { Int:0, Ptr: nullptr,    Flt:0.0f  }

	// ]

	SArr.SetNumZeroed(2);

	//SArr.GetAllocatedSize();

	//SArr.CountBytes

}

void AXGArrayActor::XGSwapArray()
{
	TArray<int32> MyInts={1,2,3,4,5};

	MyInts.Swap(0, 4);

	MyInts.SwapMemory(0,5);

	MyInts.Swap(0, 5);
}

void AXGArrayActor::XGAddDefaultArray()
{

	TArray<FXGAddStructInfo> MyInts = { 10,20,30,40,50 };

	int32 DefaultAddIndex= MyInts.AddDefaulted();

	FXGAddStructInfo& LastMyStruct= MyInts.AddDefaulted_GetRef();

	LastMyStruct.Healty+=2000;



}

void AXGArrayActor::XGRerversetArray()
{
	TArray<int32> MyInts ;

	MyInts.Reserve(3);

	MyInts.Add(1);

	MyInts.Add(2);

	MyInts.Add(3);

	MyInts.Reserve(10);

	MyInts.Reserve(2);

}

// Called when the game starts or when spawned
void AXGArrayActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGArrayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
