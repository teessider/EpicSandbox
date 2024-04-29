// Copyright (c) Andrew Bell

#include "EpicSandboxEditor.h"
#include "EpicSandboxEditorStyle.h"

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogEpicSandboxEditor)

FText FEpicSandboxEditor::EpicSandboxAssetCategory = INVTEXT("Epic Sandbox");

void FEpicSandboxEditor::StartupModule()
{
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Started"));

	FEpicSandboxEditorStyle::Initialize();
}

void FEpicSandboxEditor::ShutdownModule()
{
	//Unregister the Slate Style here too
	FEpicSandboxEditorStyle::Shutdown();
	// UE_LOG(LogEpicSandboxEditor, Warning, TEXT("Editor (for Game) Module Shutdown"));
}

IMPLEMENT_MODULE(FEpicSandboxEditor, EpicSandboxEditor)  // Needed so UE knows to implement the module! (not GAME_MODULE since it is editor module)