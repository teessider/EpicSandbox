// Fill out your copyright notice in the Description page of Project Settings.

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
