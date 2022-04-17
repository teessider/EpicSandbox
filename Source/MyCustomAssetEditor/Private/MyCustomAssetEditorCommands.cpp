// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCustomAssetEditorCommands.h"

#define LOCTEXT_NAMESPACE "MyCustomAssetEditor"

void FMyCustomAssetEditorCommands::RegisterCommands()
{
	UI_COMMAND(SetShowGrid, "Grid", "Displays the viewport grid.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SetShowBounds, "Bounds", "Toggles display of the bounds of the preview component.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SetShowCollision, "Collision", "Toggles display of the collision of the preview component", EUserInterfaceActionType::ToggleButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE