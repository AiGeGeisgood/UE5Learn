#include "XGSampleEMailAsyncAction.h"


#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"
#include "Containers/StringConv.h"
#include "Common/TcpSocketBuilder.h"


#include "LogXGSampleEMail.h"
#include "XGSampleEMailSubsystem.h"

FXGSampleEmailAsyncAction::FXGSampleEmailAsyncAction()
	:AsyncID(FGuid::NewGuid())
{
}

FXGSampleEmailAsyncAction::~FXGSampleEmailAsyncAction()
{

	CloseEmailRunnable();

	if (XGEmailRunnableThread)
	{
		XGEmailRunnableThread->WaitForCompletion();
		delete XGEmailRunnableThread;

		XGEmailRunnableThread = nullptr;

	}

	XGEmailRunnable.Reset();


}

TSharedPtr<FXGSampleEmailAsyncAction> FXGSampleEmailAsyncAction::CreateEmailAsyncAction(FXGSampleEmailInfo InEmailInfo, FXGSampleEmailDelegate InEmailDelegate)
{
	TSharedPtr<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe> XGEmailAsyncAction = MakeShared<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe>();

	XGEmailAsyncAction->XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::Init;

	XGEmailAsyncAction->EmailInfo = InEmailInfo;
	XGEmailAsyncAction->EmailDelegate = InEmailDelegate;

	XGEmailAsyncAction->Activate();

	return XGEmailAsyncAction;
}

void FXGSampleEmailAsyncAction::StopEmailAsyncAction()
{
	CloseEmailRunnable();
}

FGuid FXGSampleEmailAsyncAction::GetAsyncActionID()
{
	return AsyncID;
}

EXGSampleEMailAsyncActionStatus FXGSampleEmailAsyncAction::GetAsyncActionStatus()
{
	return XGMailAsyncActionStatus;
}

void FXGSampleEmailAsyncAction::Activate()
{
	TSharedPtr<FXGSampleEmailAsyncAction, ESPMode::ThreadSafe> AsyncAction = this->AsShared();


	AsyncTask(ENamedThreads::GameThread, [AsyncAction]() {

		AsyncAction->Activate_Internal();

		});


}

void FXGSampleEmailAsyncAction::Activate_Internal()
{

	CreateEmailRunnable();

}

void FXGSampleEmailAsyncAction::CallEmailDelegate(FGuid InAsyncID, bool bResult, FString InMessage)
{
	FXGSampleEmailDelegate TempDalegate = EmailDelegate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDalegate.Execute(InAsyncID, bResult, InMessage);

		});
}

void FXGSampleEmailAsyncAction::CreateEmailRunnable()
{
	UE_LOG(LogXGSampleEmail, Warning, TEXT("[%s],AsyncID:[%s],Begin to Create Email AsyncAction!"), *FString(__FUNCTION__), *(AsyncID.ToString()));


	XGEmailRunnable = MakeShared<FXGSampleEmailRunnable>(TEXT("XGEmailRunnable"), GetEmailServerIP(), 25);


	XGEmailRunnable->OnInit().BindSP(this, &FXGSampleEmailAsyncAction::OnInit);
	XGEmailRunnable->OnInitError().BindSP(this, &FXGSampleEmailAsyncAction::OnInitError);
	XGEmailRunnable->OnMessage().BindSP(this, &FXGSampleEmailAsyncAction::OnMessage);
	XGEmailRunnable->OnMessageSent().BindSP(this, &FXGSampleEmailAsyncAction::OnMessageSent);
	XGEmailRunnable->OnClosed().BindSP(this, &FXGSampleEmailAsyncAction::OnClosed);



	XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::Connected;

	XGEmailRunnableThread = FRunnableThread::Create(XGEmailRunnable.Get(), *(XGEmailRunnable->ThreadName));


}

void FXGSampleEmailAsyncAction::CloseEmailRunnable()
{

	if (XGEmailRunnable.IsValid())
	{
		XGEmailRunnable->Stop();

	}

	//不能在这里释放 会导致线程杀死的时候出断点
	//XGEmailRunnable.Reset();
}

void FXGSampleEmailAsyncAction::NotifyEmailSubsystemToRemoveSelf()
{
	FXGSampleEmailSubsystem* EmailSubsystem = FXGSampleEmailSubsystem::GetXGEmailSubsystem();

	EmailSubsystem->RemovePointedEmailAsyncAction(AsyncID);

}

void FXGSampleEmailAsyncAction::StageClose(const FString& InStageStr, const FString& InMessage, bool bSuccess)
{
	StageMessage = FString::Printf(TEXT("Stage:[%s],OnMessage:[%s]"), *InStageStr, *InMessage);

	CloseEmailRunnable();
}

void FXGSampleEmailAsyncAction::OnInit()
{
	UE_LOG(LogXGSampleEmail, Display, TEXT("[%s],AsyncID:[%s],Init Email AsyncAction Successfully!"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::ConnectedSuccess;


}

void FXGSampleEmailAsyncAction::OnInitError(const FString& InMessage)
{
	UE_LOG(LogXGSampleEmail, Display, TEXT("[%s],AsyncID:[%s],Init Email AsyncAction Unsuccessfully! Reason:[%s]"), *FString(__FUNCTION__), *(AsyncID.ToString()), *InMessage);


	XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::ConnectedError;

	CloseEmailRunnable();

	CallEmailDelegate(AsyncID, false, InMessage);

	NotifyEmailSubsystemToRemoveSelf();



}

void FXGSampleEmailAsyncAction::OnMessage(const FString& InMessage)
{
	UE_LOG(LogXGSampleEmail, Log, TEXT("[%s], AsyncID:[%s],OnMessage:%s"), *FString(__FUNCTION__), *(AsyncID.ToString()), *InMessage);

	switch (XGMailAsyncActionStatus)
	{
	case EXGSampleEMailAsyncActionStatus::ConnectedSuccess:

		if (InMessage.Contains(TEXT("220")))
		{
			Send(TEXT("EHLO HYL-PC\r\n"));

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::EHLO;
		}
		else
		{
			FString StageStr = TEXT("Hello Server Failed!");

			StageClose(StageStr, InMessage);
		}
		break;

	case EXGSampleEMailAsyncActionStatus::EHLO:

		if (InMessage.Contains(TEXT("250")))
		{
			Send(TEXT("AUTH LOGIN\r\n"));

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::AuthLogin_UserName;
		}
		else
		{

			FString StageStr = TEXT("EHLO Failed!");

			StageClose(StageStr, InMessage);
		}


		break;

	case EXGSampleEMailAsyncActionStatus::AuthLogin_UserName:

		if (InMessage.Contains(TEXT("334")))
		{
			FString AuthLoginStr = FBase64::Encode(EmailInfo.AuthInfo.UserName) + TEXT("\r\n");

			Send(AuthLoginStr);

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::AuthLogin_Password;


		}
		else
		{
			FString StageStr = TEXT("AUTH LOGIN Failed!");

			StageClose(StageStr, InMessage);
		}


		break;

	case EXGSampleEMailAsyncActionStatus::AuthLogin_Password:

		if (InMessage.Contains(TEXT("334")))
		{
			FString AuthCodePassword = FBase64::Encode(EmailInfo.AuthInfo.AuthCode) + TEXT("\r\n");

			Send(AuthCodePassword);

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::AuthLogin_Result;
		}
		else
		{

			FString StageStr = TEXT("AuthLogin_UserName Failed!");

			StageClose(StageStr, InMessage);
		}

		break;

	case EXGSampleEMailAsyncActionStatus::AuthLogin_Result:

		if (InMessage.Contains(TEXT("235")))
		{
			FString MailFrom = TEXT("MAIL FROM: <") + EmailInfo.ContentInfo.FromEmail + TEXT(">\r\n");


			Send(MailFrom);

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::SetMailFrom;

		}
		else
		{

			FString StageStr = TEXT("AuthLogin_Password Failed!");

			StageClose(StageStr, InMessage);


		}


		break;

	case EXGSampleEMailAsyncActionStatus::SetMailFrom:

		if (InMessage.Contains(TEXT("250")))
		{
			FString ToMail = TEXT("RCPT TO:<") + EmailInfo.ContentInfo.ToEmail + TEXT(">\r\n");



			Send(ToMail);

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::SetEmailTo;



		}
		else
		{


			FString StageStr = TEXT("AuthLogin_Result Failed!");

			StageClose(StageStr, InMessage);
		}

		break;

	case EXGSampleEMailAsyncActionStatus::SetEmailTo:

		if (InMessage.Contains(TEXT("250")))
		{
			Send(TEXT("DATA\r\n"));

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::DataPrepare;

		}
		else
		{

			FString StageStr = TEXT("SetMailFrom Failed!");

			StageClose(StageStr, InMessage);
		}


		break;

	case EXGSampleEMailAsyncActionStatus::DataPrepare:

		if (InMessage.Contains(TEXT("354")))
		{
			FString ContentStr = EmailInfo.ContentInfo.ToString() + TEXT("\r\n.\r\n");

			Send(ContentStr);

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::Data;

		}
		else
		{


			FString StageStr = TEXT("SetEmailTo Failed!");

			StageClose(StageStr, InMessage);
		}

		break;

	case EXGSampleEMailAsyncActionStatus::Data:

		if (InMessage.Contains(TEXT("250")))
		{
			Send(TEXT("QUIT\r\n"));

			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::Quit;

		}
		else
		{


			FString StageStr = TEXT("DataPrepare Failed!");

			StageClose(StageStr, InMessage);
		}



		break;
	case EXGSampleEMailAsyncActionStatus::Quit:

		if (InMessage.Contains(TEXT("221")))
		{
			XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::Finished;

			FString StageStr = TEXT("Email Finished Successfull!");

			StageClose(StageStr, InMessage, true);
		}
		else
		{


			FString StageStr = TEXT("Data Failed!");

			StageClose(StageStr, InMessage);
		}

		break;


	default:

		XGMailAsyncActionStatus = EXGSampleEMailAsyncActionStatus::UnKnowedError;

		break;




	}





}

void FXGSampleEmailAsyncAction::OnMessageSent(bool InSentResult, const FString& InMessage)
{
	//UE_LOG(LogXGSampleEmail, Log, TEXT("[%s],AsyncID:[%s],SentResult:[%s] OnMessageSent:%s"),
	//	*FString(__FUNCTION__), *(AsyncID.ToString()), InSentResult ? TEXT("true") : TEXT("false"), *InMessage);

	UEnum* EmailEnum = StaticEnum<EXGSampleEMailAsyncActionStatus>();
	FString StageStr = EmailEnum->GetNameStringByValue((int64)XGMailAsyncActionStatus);

	if (InSentResult)
	{

	}
	else
	{
		StageClose(StageStr, InMessage);
	}



}

void FXGSampleEmailAsyncAction::OnClosed(const FString& InMessage)
{
	UE_LOG(LogXGSampleEmail, Warning, TEXT("[%s],AsyncID:[%s],ConnectOnClosed,Reason:[%s]"), *FString(__FUNCTION__),
		*(AsyncID.ToString()),
		*InMessage);


	if (XGMailAsyncActionStatus == EXGSampleEMailAsyncActionStatus::Finished)
	{
		CallEmailDelegate(AsyncID, true, StageMessage);
	}
	else if (XGMailAsyncActionStatus == EXGSampleEMailAsyncActionStatus::ConnectedError)
	{
		//Do nothing has called before.
	}
	else
	{
		UEnum* EmailEnum = StaticEnum<EXGSampleEMailAsyncActionStatus>();
		FString StageStr = EmailEnum->GetNameStringByValue((int64)XGMailAsyncActionStatus);

		FString CloseMessage = TEXT("AsyncAction has not finished,Current stage is ") + StageStr + TEXT(",StageMessage is ") + StageMessage;

		CallEmailDelegate(AsyncID, false, CloseMessage);

	}

	NotifyEmailSubsystemToRemoveSelf();


}

FString FXGSampleEmailAsyncAction::GetEmailServerIP()
{

	return TEXT("120.232.69.34");
}

void FXGSampleEmailAsyncAction::Send(FString InMessage)
{
	if (XGEmailRunnable.IsValid())
	{
		XGEmailRunnable->SendMessage(InMessage);
	}

}
