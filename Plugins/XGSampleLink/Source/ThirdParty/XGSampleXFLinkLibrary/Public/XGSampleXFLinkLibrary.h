#pragma once

#include "Modules/ModuleManager.h"

class FXGSampleXFLinkLibraryModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

};
