// Copyright (c) Andrew Bell

#include "EpicSandboxEditor.h"
#include "EpicSandboxEditorStyle.h"
#include "AssetTypeActions/AssetTypeActions_MyCustomAsset.h"
#include "AssetTypeActions/AssetTypeActions_MyCustomDataAsset.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogEpicSandboxEditor)

#define LOCTEXT_NAMESPACE "EpicSandboxEditor"

EAssetTypeCategories::Type FEpicSandboxEditor::EpicSandboxAssetCategory;  // Forward Declaration

void FEpicSandboxEditor::StartupModule()
{
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Started"));

	FEpicSandboxEditorStyle::Initialize();
	RegisterAssetTypeActions();
}

void FEpicSandboxEditor::ShutdownModule()
{
	//Unregister the Slate Style here too
	FEpicSandboxEditorStyle::Shutdown();
	UnregisterAssetTypeActions();
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Shutdown"));
}

void FEpicSandboxEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

void FEpicSandboxEditor::RegisterAssetTypeActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EpicSandboxAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("EpicSandboxCategory")), LOCTEXT("EpicSandboxCategory", "EpicSandbox"));

	// In NiagaraEditorModule, these are inside a method to simplify it
	// const TSharedPtr<IAssetTypeActions> MyCustomAssetActionType = MakeShareable(new FAssetTypeActions_MyCustomAsset());
	// const TSharedPtr<IAssetTypeActions> MyCustomDataAssetActionType = MakeShareable(new FAssetTypeActions_MyCustomDataAsset());
	//
	// AssetTools.RegisterAssetTypeActions(MyCustomAssetActionType.ToSharedRef());
	// AssetTools.RegisterAssetTypeActions(MyCustomDataAssetActionType.ToSharedRef());

	// DOES THE ABOVE IN TWO LINES + Add to Cached AssetTypeActions
	RegisterAssetTypeAction(AssetTools, MakeShared<FAssetTypeActions_MyCustomAsset>());
	RegisterAssetTypeAction(AssetTools, MakeShared<FAssetTypeActions_MyCustomDataAsset>());
}

void FEpicSandboxEditor::UnregisterAssetTypeActions()
{
	// Based on NiagarEditorModule Shutdown method...
	// It's good practice to unregister the AssetTypeActions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty(); // Don't forget to empty the actions array!
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEpicSandboxEditor, EpicSandboxEditor)  // Needed so UE knows to implement the module! (not GAME_MODULE since it is editor module)
