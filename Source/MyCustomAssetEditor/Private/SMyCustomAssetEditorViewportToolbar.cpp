// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCustomAssetEditorViewportToolbar.h"
#include "SMyCustomAssetEditorViewport.h"
#include "MyCustomAssetEditorCommands.h"

void SMyCustomAssetEditorViewportToolbar::Construct(const FArguments& InArgs, TSharedPtr<ICommonEditorViewportToolbarInfoProvider> InInfoProvider)
{
	SCommonEditorViewportToolbarBase::Construct(SCommonEditorViewportToolbarBase::FArguments(), InInfoProvider);
}

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateShowMenu() const
{
	GetInfoProvider().OnFloatingButtonClicked();

	// This reference is for SMyCustomAssetEditorViewport...
	TSharedRef<SEditorViewport> ViewportRef = GetInfoProvider().GetViewportWidget();

	const bool bInShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder ShowMenuBuilder(bInShouldCloseWindowAfterMenuSelection, ViewportRef->GetCommandList());
	{
		const FMyCustomAssetEditorCommands& Commands = FMyCustomAssetEditorCommands::Get();

		ShowMenuBuilder.AddMenuEntry(Commands.SetShowGrid);
		ShowMenuBuilder.AddMenuEntry(Commands.SetShowBounds);
		ShowMenuBuilder.AddMenuEntry(Commands.SetShowCollision);
	}

	return ShowMenuBuilder.MakeWidget();
}
