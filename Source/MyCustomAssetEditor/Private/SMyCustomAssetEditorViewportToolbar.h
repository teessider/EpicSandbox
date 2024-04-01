// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SViewportToolBar.h"

class SMyCustomAssetEditorViewport;

/**
 * The level viewport toolbar widget that is placed in the viewport
 */
class SMyCustomAssetEditorViewportToolbar : public SViewportToolBar
{
public:
	SLATE_BEGIN_ARGS(SMyCustomAssetEditorViewportToolbar) {}
	SLATE_ARGUMENT(TArray<TSharedPtr<FExtender>>, Extenders)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<SMyCustomAssetEditorViewport> InViewport);

private:

	/**
	* Generates the Show menu content
	*/
	TSharedRef<SWidget> GenerateShowMenu() const;
	
	/** The viewport that we are in */
	TWeakPtr<SMyCustomAssetEditorViewport> Viewport;

	/** Command list */
	TSharedPtr<FUICommandList> CommandList;

	/** Extenders */
	TArray<TSharedPtr<FExtender>> Extenders;
};
