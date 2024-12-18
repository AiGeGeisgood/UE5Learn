// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleEMail.h"
#include "XGSampleEMailSubsystem.h"
#define LOCTEXT_NAMESPACE "FXGSampleEMailModule"

void FXGSampleEMailModule::StartupModule()
{
	FXGSampleEmailSubsystem::GetXGEmailSubsystem();
	
}

void FXGSampleEMailModule::ShutdownModule()
{
	FXGSampleEmailSubsystem::RealeaseXGEmailSubsystem();
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXGSampleEMailModule, XGSampleEMail)