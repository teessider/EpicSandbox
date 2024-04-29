// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEpicSandboxEditor, All, All)

class EPICSANDBOXEDITOR_API FEpicSandboxEditor final : public IModuleInterface
{
public:
	// Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface implementation

	static FText GetAssetCategoryName() { return EpicSandboxAssetCategory; }

private:
	static FText EpicSandboxAssetCategory;
};
