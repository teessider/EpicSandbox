// Copyright (c) Andrew Bell

#pragma once

#include "Modules/ModuleInterface.h"

class EPICSANDBOX_API FEpicSandbox : public IModuleInterface
{
public:
	// Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface implementation
};
