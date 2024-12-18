#include "XGLogActor.h"
#include "XGLogType.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogXGLogActor, Log, All)

// Sets default values
AXGLogActor::AXGLogActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AXGLogActor::XGLog()
{
	UE_LOG(LogXGLogActor, Log, TEXT("This is Log"));


	UE_LOG(LogXGSample, Error, TEXT("This is Error"));
	UE_LOG(LogXGSample, Warning, TEXT("This is Warning"));
	UE_LOG(LogXGSample, Display, TEXT("This is Display"));
	UE_LOG(LogXGSample, Log, TEXT("This is Log"));
	UE_LOG(LogXGSample, Verbose, TEXT("This is Verbose"));
	UE_LOG(LogXGSample, VeryVerbose, TEXT("This is VeryVerbose"));


	//UE_LOG(LogXGSample, Fatal, TEXT("This is Fatal"));
	
}

void AXGLogActor::XGLogString()
{
	int32 MyInt = 32;
	FString MyString = TEXT("Test");
	float MyFloat = 32.2f;

	UE_LOG(LogXGSample, Error, TEXT("int32: [%d] ,FString:[%s],float:[%f]"), MyInt, *MyString, MyFloat);

	FString MyNewString = FString::Printf(TEXT("New---int32: [%d] ,FString:[%s],float:[%.2f]"), MyInt, *MyString, MyFloat);

	UE_LOG(LogXGSample, Error, TEXT("[%s]"), *MyNewString);

	UE_LOG(LogXGSample, Error, TEXT("[%s]"), *FString(__FUNCTION__));

	PringLogString(MyNewString);

	

}

void AXGLogActor::XGLogFormatString()
{
	FString MyName =TEXT("XG");
	int32 ErrorCode = 998;
	FString LoadFlags = TEXT("dFlags");

	UE_LOGFMT(LogXGSample, Warning, "Loading `{Name}` failed with error {Error}", MyName, ErrorCode);

	UE_LOGFMT(LogXGSample, Warning, "Loading `{Name}` failed with error {Error}", ("Name", MyName), ("Error", ErrorCode), ("Flags", LoadFlags));


}

void AXGLogActor::PringLogString(FString InStr)
{
	//假定我们是在其他线程

	AsyncTask(ENamedThreads::GameThread, [InStr]() {

		UE_LOG(LogXGSample, Error, TEXT("Async--[%s]"), *InStr);


		});


}

// Called when the game starts or when spawned
void AXGLogActor::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)

	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("This is an Example on-screen debug message."));

	}
}

// Called every frame
void AXGLogActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
