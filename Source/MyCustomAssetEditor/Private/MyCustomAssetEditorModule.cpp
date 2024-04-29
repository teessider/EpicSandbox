// Copyright (c) Andrew Bell

#include "MyCustomAssetEditorModule.h"
#include "MyCustomAssetEditorCommands.h"

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogMyCustomAssetEditor);

#define LOCTEXT_NAMESPACE "MyCustomAssetEditor"

void FMyCustomAssetEditorModule::StartupModule()
{
	UE_LOG(LogMyCustomAssetEditor, Warning, TEXT("My Custom Asset Editor (for Game) Module Started"));
	
	FMyCustomAssetEditorCommands::Register();

	// INITIALIZE ASSET EDITOR SLATE STYLE HERE IF NECESSARY
}

void FMyCustomAssetEditorModule::ShutdownModule()
{
	FMyCustomAssetEditorCommands::Unregister();
	
	// UNREGISTER ASSET EDITOR SLATE STYLE HERE TOO
	UE_LOG(LogMyCustomAssetEditor, Warning, TEXT("My Custom Asset Editor (for Game) Module Shutdown"));
}

IMPLEMENT_MODULE(FMyCustomAssetEditorModule, MyCustomAssetEditor)  // Needed so UE knows to implement the module! (not GAME_MODULE since it is editor module)

#undef LOCTEXT_NAMESPACE

