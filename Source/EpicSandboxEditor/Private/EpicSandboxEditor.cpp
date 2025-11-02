// Copyright (c) Andrew Bell

#include "EpicSandboxEditor.h"
#include "EpicSandboxEditorStyle.h"
#include "MyCustomActor.h"
#include "Factories/MyCustomAssetActorFactory.h"
#include "EpicSandboxEditorSettingsCustomization.h"

#include "Modules/ModuleManager.h"
#include "IPlacementModeModule.h"
#include "Subsystems/PlacementSubsystem.h"

DEFINE_LOG_CATEGORY(LogEpicSandboxEditor)

FText FEpicSandboxEditor::EpicSandboxAssetCategory = INVTEXT("Epic Sandbox");

void FEpicSandboxEditor::StartupModule()
{
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Started"));

	FEpicSandboxEditorStyle::Initialize();
	RegisterAssetFactories();
	RegisterAssetPlacement();
	RegisterDetailsCustomizations();
}

void FEpicSandboxEditor::ShutdownModule()
{
	//Unregister the Slate Style here too
	FEpicSandboxEditorStyle::Shutdown();
	UnregisterAssetFactories();
	UnregisterDetailsCustomizations();
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Shutdown"));
}

void FEpicSandboxEditor::RegisterAssetFactories()
{
	if(!GEditor)
	{
		return;
	}
	
	if (UPlacementSubsystem* PlacementSubsystem = GEditor->GetEditorSubsystem<UPlacementSubsystem>())
	{
		PlacementSubsystem->RegisterAssetFactory(NewObject<UMyCustomAssetActorFactory>());
	}
}

void FEpicSandboxEditor::RegisterAssetPlacement()
{
	const IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	const FPlacementCategoryInfo Info = *PlacementModeModule.GetRegisteredPlacementCategory(FBuiltInPlacementCategories::Basic());

	IPlacementModeModule::Get().RegisterPlaceableItem(Info.UniqueHandle, MakeShared<FPlaceableItem>(
		*AMyCustomActor::StaticClass(),
		FAssetData(AMyCustomActor::StaticClass())));
}

void FEpicSandboxEditor::RegisterDetailsCustomizations()
{
	static const FName PropertyEditorModuleName("PropertyEditor");
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorModuleName);
	
	PropertyEditorModule.RegisterCustomClassLayout("EpicSandboxEditorSettings", FOnGetDetailCustomizationInstance::CreateStatic(&FEpicSandboxEditorSettingsCustomization::MakeInstance));
}

void FEpicSandboxEditor::UnregisterDetailsCustomizations()
{
	static const FName PropertyEditorModuleName("PropertyEditor");
	if (FModuleManager::Get().IsModuleLoaded(PropertyEditorModuleName))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditorModuleName);
		
		PropertyEditorModule.UnregisterCustomClassLayout("EpicSandboxEditorSettings");
	}
}

void FEpicSandboxEditor::UnregisterAssetFactories()
{
	if(!GEditor)
	{
		return;
	}
	
	if (UPlacementSubsystem* PlacementSubsystem = GEditor->GetEditorSubsystem<UPlacementSubsystem>())
	{
		PlacementSubsystem->UnregisterAssetFactory(NewObject<UMyCustomAssetActorFactory>());
	}
}

IMPLEMENT_MODULE(FEpicSandboxEditor, EpicSandboxEditor)  // Needed so UE knows to implement the module! (not GAME_MODULE since it is editor module)
