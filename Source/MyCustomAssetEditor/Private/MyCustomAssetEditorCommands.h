// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"


class FMyCustomAssetEditorCommands : public TCommands<FMyCustomAssetEditorCommands>
{
public:
	FMyCustomAssetEditorCommands()
		: TCommands<FMyCustomAssetEditorCommands>(
			TEXT("MyCustomAssetEditor"), // Context name for fast lookup
			NSLOCTEXT("Contexts", "MyCustomAssetEditor", "My Custom Asset Editor"),
			NAME_None, // No Parent Context. Most Commands classes do NOT have a parent context. StaticMeshEditorCommands does though
			FEditorStyle::GetStyleSetName() // Icon Style Set
			)
	{
	}

	// TCommand<> interface
	virtual void RegisterCommands() override;
	// End of TCommand<> interface
	
	TSharedPtr<FUICommandInfo> SetShowGrid;
	TSharedPtr<FUICommandInfo> SetShowBounds;
	TSharedPtr<FUICommandInfo> SetShowCollision;
	
};
