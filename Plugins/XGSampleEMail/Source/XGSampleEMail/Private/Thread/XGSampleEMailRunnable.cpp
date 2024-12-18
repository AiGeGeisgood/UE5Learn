#include "XGSampleEMailRunnable.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/TcpSocketBuilder.h"


FXGSampleEmailRunnable::FXGSampleEmailRunnable(FString InThreadName, FString InServerIP, int32 InServerPort)
	:ThreadName(InThreadName),
	ServerIP(InServerIP),
	ServerPort(InServerPort)
{


}

FXGSampleEmailRunnable::~FXGSampleEmailRunnable()
{
}

bool FXGSampleEmailRunnable::Init()
{
	return true;
}

uint32 FXGSampleEmailRunnable::Run()
{

	ConnectSocket = FTcpSocketBuilder(TEXT("Email Client Socket"))
		.AsReusable()
		.AsBlocking()
		.WithReceiveBufferSize(ReceiveSize)
		.WithSendBufferSize(SendSize);

	if (!ConnectSocket)
	{

		CallInitErrorDelegate(TEXT("TcpSocket Create Failed"));

		return 0;

	}
	else
	{

		FString Domain = TEXT("smtp.qq.com");

		ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		TSharedRef<FInternetAddr> Addr = SocketSubSystem->CreateInternetAddr(0, 0);

		ESocketErrors SocketError = SocketSubSystem->GetHostByName(TCHAR_TO_ANSI(*Domain), *Addr);

		if (SocketError != SE_NO_ERROR)
		{
			CallInitErrorDelegate(TEXT("Ip Query Failed"));

			return 0;
		}

		Addr->SetPort(ServerPort);

		bool bConnect = ConnectSocket->Connect(*Addr);

		if (bConnect)
		{
			CallInitDelegate();
		}
		else
		{
			FString InitErrorMessage = FString::Printf(TEXT("TcpSocket Init Failed,Connect is failed,EmailServerIP:[%s],Port:[%d]"), *ServerIP, ServerPort);

			CallInitErrorDelegate(InitErrorMessage);

			return 0;
		}

	}

	while (bIsRunning)
	{
		uint32 Size;

		bool LostConnect = false;

		ConnectSocket->HasPendingConnection(LostConnect);

		ConnectSocket->Wait(ESocketWaitConditions::WaitForReadOrWrite, FTimespan(0, 0, 5));

		if (LostConnect)
		{
			break;
		}

		if (ConnectSocket && ConnectSocket->HasPendingData(Size))
		{
			ReceiveData.Init(0, FMath::Min(Size, ReceiveSize));

			int32 ReadedData;

			ConnectSocket->Recv(ReceiveData.GetData(), ReceiveSize, ReadedData);

			FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData())));

			CallReceiveDelegate(ReceivedString);

		}


	}


	return 0;
}

void FXGSampleEmailRunnable::Stop()
{
	bIsRunning = false;
}

void FXGSampleEmailRunnable::Exit()
{

	if (ConnectSocket)
	{
		ConnectSocket->Close();

		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectSocket);

		ConnectSocket = nullptr;

		CallCloseDelegate(TEXT("ConnectSocket has been closed !"));
	}


}

FXGEmailSocketInitDelegate& FXGSampleEmailRunnable::OnInit()
{
	return InitDelegate;
}

FXGEmailSocketInitErrorDelegate& FXGSampleEmailRunnable::OnInitError()
{
	return InitErrorDelegate;
}

FXGEmailSocketReceiveDelegate& FXGSampleEmailRunnable::OnMessage()
{
	return ReceiveDelegate;
}

FXGEmailSocketSendDelegate& FXGSampleEmailRunnable::OnMessageSent()
{
	return SendDelegate;
}

FXGEmailSocketCloseDelegate& FXGSampleEmailRunnable::OnClosed()
{
	return ClosedDelegate;
}

void FXGSampleEmailRunnable::SendMessage(const FString& InMessage)
{
	//Execute on GameThread.

	const TCHAR* MessageTCharPointer = InMessage.GetCharArray().GetData();

	uint32 MessageSize = strlen(TCHAR_TO_ANSI(MessageTCharPointer));

	int32 ByteSent = 0;

	if (MessageSize >= SendSize)
	{
		CallSendDelegate(false, TEXT("Send Data Size is Larger than Max Size for set"));
	}
	else
	{
		if (ConnectSocket && ConnectSocket->Send((uint8*)TCHAR_TO_ANSI(MessageTCharPointer), MessageSize, ByteSent))
		{
			//TODO There are some problems to check . Fix next version.

			FString ConvertedStr = ANSI_TO_TCHAR(TCHAR_TO_ANSI(MessageTCharPointer));

			CallSendDelegate(true, ConvertedStr);

		}
		else
		{
			FString ConvertedStr = ANSI_TO_TCHAR(TCHAR_TO_ANSI(MessageTCharPointer));
			FString FailedMessage = TEXT("MessageSent is failed, Message:") + ConvertedStr;
			CallSendDelegate(false, ConvertedStr);
		}

	}




}

void FXGSampleEmailRunnable::CallInitDelegate()
{
	FXGEmailSocketInitDelegate TempDelegate = InitDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound();
		});
}

void FXGSampleEmailRunnable::CallInitErrorDelegate(const FString& InMessage)
{
	FXGEmailSocketInitErrorDelegate TempDelegate = InitErrorDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InMessage);

		});
}

void FXGSampleEmailRunnable::CallReceiveDelegate(const FString& InMessage)
{
	FXGEmailSocketReceiveDelegate TempDelegate = ReceiveDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InMessage);
		});
}

void FXGSampleEmailRunnable::CallSendDelegate(bool InInitResult, const FString& InMessage)
{
	FXGEmailSocketSendDelegate TempDelegate = SendDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InInitResult, InMessage);

		});
}

void FXGSampleEmailRunnable::CallCloseDelegate(const FString& InMessage)
{
	FXGEmailSocketCloseDelegate TempDelegate = ClosedDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.ExecuteIfBound(InMessage);
		});
}
