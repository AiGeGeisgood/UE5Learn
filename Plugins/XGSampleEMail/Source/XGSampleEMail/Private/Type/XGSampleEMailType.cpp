#include "XGSampleEMailType.h"

FString FXGSampleEmailContentInfo::ToString()
{
    FString EmailContent = TEXT("");

    EmailContent = FString::Printf(TEXT("From: \"%s\"<%s>\r\nTo: \"%s\"<%s>\r\nSubject: %s\r\n\r\n%s\n"),
    *FromName, *FromEmail, *ToName, *ToEmail, *Subject, *Content);



    return EmailContent;
}
