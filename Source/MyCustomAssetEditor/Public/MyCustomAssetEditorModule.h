// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Toolkits/AssetEditorToolkit.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyCustomAssetEditor, All, All)

class MYCUSTOMASSETEDITOR_API FMyCustomAssetEditorModule final : public IModuleInterface
{
public:
	// Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface implementation

	TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() const { return MenuExtensibilityManager; }
	TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() const { return ToolBarExtensibilityManager; }

private:
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};