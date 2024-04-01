// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCustomAssetEditorCommands.h"

#define LOCTEXT_NAMESPACE ""

void FMyCustomAssetEditorCommands::RegisterCommands()
{
	// Only Commands unique to this editor should go in here
	// Previously the Show Grid, Bounds & Collision commands were here
	// but they are now handled elsewhere: FShowFlagMenuCommands & FEditorViewportCommands
}

#undef LOCTEXT_NAMESPACE