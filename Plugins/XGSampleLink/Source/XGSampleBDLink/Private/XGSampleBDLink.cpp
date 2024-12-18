// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleBDLink.h"

#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"


#define LOCTEXT_NAMESPACE "FXGSampleBDLinkModule"

void FXGSampleBDLinkModule::StartupModule()
{
	
}

void FXGSampleBDLinkModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXGSampleBDLinkModule, XGSampleBDLink)
