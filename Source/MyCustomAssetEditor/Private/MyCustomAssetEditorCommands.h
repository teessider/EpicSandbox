// Copyright (c) Andrew Bell

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
			INVTEXT("My Custom Asset Editor"),
			NAME_None, // No Parent Context. Most Commands classes do NOT have a parent context. StaticMeshEditorCommands does though
			FAppStyle::GetAppStyleSetName() // Icon Style Set
			)
	{
	}

	// TCommand<> interface
	virtual void RegisterCommands() override;
	// End of TCommand<> interface
	
	// for each unique command a TSharedPtr<FUICommandInfo> should be made
	// Example:
	// TSharedPtr<FUICommandInfo> SetShowCoolViewportVisualization
};
