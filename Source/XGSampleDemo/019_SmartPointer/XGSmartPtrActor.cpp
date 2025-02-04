#include "XGSmartPtrActor.h"

// Sets default values
AXGSmartPtrActor::AXGSmartPtrActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGSmartPtrActor::InitMySPStruct()
{
	TSharedPtr<FSmartPtrStruct> MySmartPtrStruct0;


	TSharedPtr<FSmartPtrStruct> MySmartPtrStruct1 = MakeShareable(new FSmartPtrStruct());

	TSharedRef<FSmartPtrStruct> MySmartPtrStruct2 = MakeShared<FSmartPtrStruct>();

	//TSharedRef<FSmartPtrStruct> MySmartPtrStruct3;

}

void AXGSmartPtrActor::InitSPStruct()
{

	// 创建一个空白的共享指针

	TSharedPtr<FSmartPtrStruct> EmptyPointer;

	// 为新对象创建一个共享指针

	TSharedPtr<FSmartPtrStruct> NewPointer(new FSmartPtrStruct(123));

	// 从共享引用创建一个共享指针

	TSharedRef<FSmartPtrStruct> NewReference(new FSmartPtrStruct(456));

	TSharedPtr<FSmartPtrStruct> PointerFromReference = NewReference;

	PointerFromReference->aa = 4567;

	// 创建一个线程安全的共享指针

	TSharedPtr<FSmartPtrStruct, ESPMode::ThreadSafe> NewThreadsafePointer = MakeShared<FSmartPtrStruct, ESPMode::ThreadSafe>(998);





}

void AXGSmartPtrActor::ResetSPStruct()
{

	TSharedPtr<FSmartPtrStruct> PointerOne(new FSmartPtrStruct(1111));

	TSharedPtr<FSmartPtrStruct> PoninterTwo(new FSmartPtrStruct(2222));

	PointerOne.Reset();

	PoninterTwo = nullptr;

	int32 a = 1;

	int32 b = 1;

	int32 c = 1;

}

void AXGSmartPtrActor::MoveSPStruct()
{


	TSharedPtr<FSmartPtrStruct> PointerOne(new FSmartPtrStruct(1111));

	TSharedPtr<FSmartPtrStruct> PoninterTwo;

	// 将PointerOne的内容移至PointerTwo。在此之后，PointerOne将引用nullptr。

	PoninterTwo = MoveTemp(PointerOne);

	// 将PointerTwo的内容移至PointerOne。在此之后，PointerTwo将引用nullptr。

	PointerOne = MoveTempIfPossible(PoninterTwo);

	int32 b = 1;

	int32 c = 1;


}

void AXGSmartPtrActor::ConvertSPStruct()
{

	TSharedRef<FSmartPtrStruct> NewReference(new FSmartPtrStruct(456));

	TSharedPtr<FSmartPtrStruct> MySharedPointer = NewReference;

	if (MySharedPointer.IsValid())
	{
		TSharedRef<FSmartPtrStruct> BackReference = MySharedPointer.ToSharedRef();
	}

	MySharedPointer.Reset();

	TSharedRef<FSmartPtrStruct> BackReferenceNuLL = MySharedPointer.ToSharedRef();


}

void AXGSmartPtrActor::EQSPStruct()
{
	TSharedPtr<FSmartPtrStruct> NodeA, NodeB;

	if (NodeA == NodeB)
	{

		// ...

	}

	TSharedRef<FSmartPtrStruct> NodeA1(new FSmartPtrStruct(456));

	NodeB= NodeA1;

	if (NodeA1 == NodeB)
	{

		UE_LOG(LogTemp,Warning,TEXT("相等"));

	}

	if (NodeA.IsValid())

	{

		UE_LOG(LogTemp, Warning, TEXT("NodeA可用"));

	}

	if (NodeA)

	{

		UE_LOG(LogTemp, Warning, TEXT("NodeA可用"));

	}

	if (NodeA.Get() != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("NodeA可用"));

	}

	if (NodeB)
	{
		NodeB->PrintAA();

		NodeB.Get()->PrintAA();

		(*NodeB).PrintAA();

	}
	
}

void AXGSmartPtrActor::DeleteStruct()
{
	//自定义删除器，会执行删除函数而不调用析构函数
	TSharedRef<FSmartPtrStruct> NewReference(new FSmartPtrStruct(), [](FSmartPtrStruct* Obj) {
		UE_LOG(LogTemp, Warning, TEXT("自定义删除器-共享引用"));
	
		Obj->aa=558;
	});

	TSharedPtr<FSmartPtrStruct> NewPointer(new FSmartPtrStruct(), [](FSmartPtrStruct* Obj) {
	
		UE_LOG(LogTemp, Warning, TEXT("自定义删除器-共享指针"));
		Obj->aa = 559;

	});

	//不能这样写
	//auto MyLambda = [](FSmartPtrStruct* Obj) {

	//	UE_LOG(LogTemp, Warning, TEXT("自定义删除器-MyLambda"));
	//	Obj->aa = 660;
	//	};

	//TSharedPtr<FSmartPtrStruct> NewPointer2(new FSmartPtrStruct(), MyLambda);


}

void AXGSmartPtrActor::MyRef()
{
	//创建新节点的共享引用
	TSharedRef<FSmartPtrStruct> NewReference = MakeShared<FSmartPtrStruct>();

	//运行时断点崩溃
	//TSharedRef<FSmartPtrStruct> UnassignedReference;

	TSharedRef<FSmartPtrStruct> UnassignedReference= NewReference;

	TSharedRef<FSmartPtrStruct> UnassignedReference2 = MakeShared<FSmartPtrStruct>();
	//无法编译
	//TSharedRef<FSmartPtrStruct> NullAssignedReference = nullptr;

	//以下会编译，但如NullObject实际为空则断言。
	//TSharedRef<FSmartPtrStruct> NullAssignedReference = NULL;

	if (UnassignedReference == NewReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("我们是相等的"));
	}

	if (UnassignedReference2!=UnassignedReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("我们是不相等的"));
	}

	UnassignedReference2 = UnassignedReference;

	if (UnassignedReference2 == UnassignedReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("我们是又相等的"));
	}


}

void AXGSmartPtrActor::MyWeakPtr()
{
	//分配新的数据对象，并创建对其的强引用。

	TSharedRef<FSmartPtrStruct> ObjectOwnerRef = MakeShared<FSmartPtrStruct>();

	//创建指向新数据对象的弱指针。

	TWeakPtr<FSmartPtrStruct> ObjectObserver(ObjectOwnerRef);

	//假设ObjectOwner是其对象的唯一拥有者，ObjectOwner停止引用该对象时，该对象将被销毁。

	TSharedPtr<FSmartPtrStruct> ObjectOwnerPtr = ObjectOwnerRef;

	ObjectOwnerPtr.Reset();

	//ObjectOwner引用空对象，因此Pin()生成的共享指针将也将为空。被视为布尔时，空白共享指针的值为false。

	if (ObjectObserver.Pin())
	{

		//只当ObjectOwner非对象的唯一拥有者时，此代码才会运行。

		check(false);

	}






}

void AXGSmartPtrActor::MyWeakPtr2()
{
	TSharedRef<FSmartPtrStruct> ObjectOwnerRef = MakeShared<FSmartPtrStruct>();
	TWeakPtr<FSmartPtrStruct> ObjectObserver(ObjectOwnerRef);

	TWeakPtr<FSmartPtrStruct> AnotherObjectObserver1 = ObjectObserver;
	TWeakPtr<FSmartPtrStruct> AnotherObjectObserver2 = ObjectObserver;

	//可通过将弱指针设为nullptr进行重置。

	AnotherObjectObserver1 = nullptr;

	//也可使用重置函数。

	AnotherObjectObserver2.Reset();



	//获取弱指针中的共享指针，并检查其是否引用有效对象。

	if (TSharedPtr<FSmartPtrStruct> LockedObserver = ObjectObserver.Pin())
	{

		//共享指针仅在此范围内有效。

		//该对象已被验证为存在，而共享指针阻止其被删除。

		LockedObserver->PrintAA();

	}

	if (ObjectObserver.IsValid())
	{
		ObjectObserver.Pin().Get() -> PrintAA();

		//ObjectOwnerPtr.Reset();

	}





}

void AXGSmartPtrActor::MyLoopPtr()
{
	TSharedRef<FSmartPtrStruct> ObjectOwnerA = MakeShared<FSmartPtrStruct>();
	TSharedRef<FSmartPtrStruct> ObjectOwnerB = MakeShared<FSmartPtrStruct>();

	ObjectOwnerA->MyHoldPtr = ObjectOwnerB;
	ObjectOwnerB->MyHoldPtr = ObjectOwnerA;


}

void AXGSmartPtrActor::MyLoopPtr2()
{
	TSharedRef<FSmartPtrStruct> ObjectOwnerA = MakeShared<FSmartPtrStruct>();
	TSharedRef<FSmartPtrStruct> ObjectOwnerB = MakeShared<FSmartPtrStruct>();

	ObjectOwnerA->MyHoldWeakPtr = ObjectOwnerB;

	ObjectOwnerB->MyHoldWeakPtr = ObjectOwnerA;

}

void AXGSmartPtrActor::MyUniquePtr()
{
	TUniquePtr<FSmartPtrStruct> MyPrt = MakeUnique<FSmartPtrStruct>();
	//TUniquePtr<FSmartPtrStruct> MyPrtCopy = MyPrt;
	TUniquePtr<FSmartPtrStruct> MyPrtMove = MoveTemp(MyPrt);

	int32 a=1;

}

// Called when the game starts or when spawned
void AXGSmartPtrActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AXGSmartPtrActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
