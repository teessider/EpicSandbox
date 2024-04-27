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

	/* Generates the toolbar view menu content */
	TSharedRef<SWidget> GenerateViewMenu() const;

	/** Returns the label for the Viewport type toolbar menu, which changes depending on the current selected type
	 * @return	Label to use for this Menu
	 */
	FText GetCameraMenuLabel() const;

	/** Returns the label icon for the Camera toolbar menu, which changes depending on viewport type 
	 * @return	Label icon to use for this menu label
	 */
	const FSlateBrush* GetCameraMenuLabelIcon() const;

	/* Generates the toolbar viewport type menu content */
	TSharedRef<SWidget> GenerateViewportTypeMenu() const;

	/** Returns the label for the Viewport type toolbar menu, which changes depending on the current selected type
	 * @return	Label to use for this Menu
	 */
	FText GetBufferViewsMenuLabel() const;
	
	/** Returns the label icon for the Camera toolbar menu, which changes depending on viewport type 
	 * @return	Label icon to use for this menu label
	 */
	const FSlateBrush* GetBufferViewsMenuLabelIcon() const;

	/* Generates the toolbar Buffer views menu content */
	TSharedRef<SWidget> GenerateBufferViewsMenu() const;
	
	/* Generates the Show menu content */
	TSharedRef<SWidget> GenerateShowMenu() const;

	/** Called by the FOV slider in the perspective viewport to get the FOV value */
	TOptional<float> OnGetFOVValue() const;
	/** Called when the FOV slider is adjusted in the perspective viewport */
	void OnFOVValueChanged(float NewValue);
	/** Build the FOV widget */
	TSharedRef<SWidget> MakeFOVWidget() const;
	
	/** The viewport that we are in */
	TWeakPtr<SMyCustomAssetEditorViewport> Viewport;

	/** Command list */
	TSharedPtr<FUICommandList> CommandList;

	/** Extenders */
	TArray<TSharedPtr<FExtender>> Extenders;
};
