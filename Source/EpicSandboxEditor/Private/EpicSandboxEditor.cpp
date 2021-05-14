// Fill out your copyright notice in the Description page of Project Settings.

#include "EpicSandboxEditor.h"
#include "AssetTypeActions/AssetTypeActions_MyCustomAsset.h"
#include "AssetTypeActions/AssetTypeActions_MyCustomDataAsset.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyleRegistry.h"

DEFINE_LOG_CATEGORY(LogEpicSandboxEditor)

#define LOCTEXT_NAMESPACE "EpicSandboxEditor"

EAssetTypeCategories::Type FEpicSandboxEditor::EpicSandboxAssetCategory;  // Forward Declaration

void FEpicSandboxEditor::StartupModule()
{
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Started"));

	// START OF SLATE STUFF
	if (!EpicSandboxEditorStyle.IsValid())
	{
		EpicSandboxEditorStyle = MakeShareable(new FSlateStyleSet("EpicSandboxStyle"));

		// For now though the default one(s) are used (the editor folders)
		//F:\SandboxEngine\Engine\Content\Editor\Slate\Icons\AssetIcons <- location of editor icons
		EpicSandboxEditorStyle->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
		EpicSandboxEditorStyle->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate")); 

		const FVector2D Icon16x16(16.0f, 16.0f);
		const FVector2D Icon64x64(64.0f, 64.0f);

		// TODO: Turn the SlateImageBrush() into macro IMAGE_BRUSH like in other places
		EpicSandboxEditorStyle->Set("ClassIcon.MyCustomAsset", new FSlateImageBrush(EpicSandboxEditorStyle->RootToContentDir(TEXT("Icons/AssetIcons/DataAsset_16x.png")), Icon16x16));
		EpicSandboxEditorStyle->Set("ClassThumbnail.MyCustomAsset", new FSlateImageBrush(EpicSandboxEditorStyle->RootToContentDir(TEXT("Icons/AssetIcons/DataAsset_64x.png")), Icon64x64));
			
		FSlateStyleRegistry::RegisterSlateStyle(*EpicSandboxEditorStyle); // Make sure to register the style!
	}
	// END OF SLATE STUFF

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	EpicSandboxAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("EpicSandboxCategory")), LOCTEXT("EpicSandboxCategory", "EpicSandbox"));

	// In NiagaraEditorModule, these are inside a method to simplify it
	// const TSharedPtr<IAssetTypeActions> MyCustomAssetActionType = MakeShareable(new FAssetTypeActions_MyCustomAsset());
	// const TSharedPtr<IAssetTypeActions> MyCustomDataAssetActionType = MakeShareable(new FAssetTypeActions_MyCustomDataAsset());
	//
	// AssetTools.RegisterAssetTypeActions(MyCustomAssetActionType.ToSharedRef());
	// AssetTools.RegisterAssetTypeActions(MyCustomDataAssetActionType.ToSharedRef());

	// DOES THE ABOVE IN TWO LINES + Add to Cached AssetTypeActions
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_MyCustomAsset()));
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_MyCustomDataAsset()));

}

void FEpicSandboxEditor::ShutdownModule()
{
	// SLATE STUFF
	//Unregister the Slate Style here too
	if (EpicSandboxEditorStyle.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(EpicSandboxEditorStyle->GetStyleSetName());
	}
	// END SLATE STUFF
	
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
	CreatedAssetTypeActions.Empty();
	
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Shutdown"));
}

void FEpicSandboxEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEpicSandboxEditor, EpicSandboxEditor)  // Needed so UE knows to implement the module! (not GAME_MODULE since it is editor module)