// Copyright Epic Games, Inc. All Rights Reserved.

#include "XGSampleXFLink.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"


#define LOCTEXT_NAMESPACE "FXGSampleXFLinkModule"

void FXGSampleXFLinkModule::StartupModule()
{
	
}

void FXGSampleXFLinkModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXGSampleXFLinkModule, XGSampleXFLink)
