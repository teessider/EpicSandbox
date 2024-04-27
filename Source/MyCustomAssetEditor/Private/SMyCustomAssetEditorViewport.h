// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "SEditorViewport.h"
#include "AdvancedPreviewScene.h"

class SMyCustomAssetEditorViewportToolbar;
class FMyCustomAssetEditor;
class FMyCustomAssetEditorViewportClient;

class SDockTab;
class SVerticalBox;

class UMyCustomAsset;


/**
 * MyCustomAsset Editor Preview viewport widget
 */
class SMyCustomAssetEditorViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SMyCustomAssetEditorViewport) {}
		SLATE_ARGUMENT(TWeakPtr<FMyCustomAssetEditor>, MyCustomAssetEditor)
		SLATE_ARGUMENT(TArray<TSharedPtr<FExtender>>, Extenders)
		SLATE_ARGUMENT(TObjectPtr<UMyCustomAsset>, ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	SMyCustomAssetEditorViewport();
	

	// FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	// End of FGCObject interface

	/** Set the parent tab of the viewport for determining visibility */
	void SetParentTab(TSharedRef<SDockTab> InParentTab) { ParentTab = InParentTab; }

	/** Returns the preview scene being rendered in the viewport */
	TSharedRef<FAdvancedPreviewScene> GetPreviewScene() const { return PreviewScene.ToSharedRef(); }

	void RefreshViewport();
	
	void ResetCamera();

	TObjectPtr<UStaticMeshComponent> GetPreviewMeshComponent() const { return PreviewMeshComponent; }

protected:
	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual EVisibility OnGetViewportContentVisibility() const override { return IsVisible() ? EVisibility::Visible : EVisibility::Collapsed; }
	virtual void BindCommands() override;
	virtual void OnFocusViewportToSelection() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

private:
	/** Determines the visibility of the viewport. */
	virtual bool IsVisible() const override;
	
	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;

	/** Pointer back to the MyCustomAsset Editor tool that owns us */
	TWeakPtr<FMyCustomAssetEditor> MyCustomAssetEditorPtr;

	/** The scene for this viewport. */
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;

	/** MyCustomAssetEditor viewport client */
	TSharedPtr<FMyCustomAssetEditorViewportClient> EditorViewportClient;

	/** The currently selected view mode. */
	EViewModeIndex CurrentViewMode;
	
	/** The viewport toolbar */
	TSharedPtr<SMyCustomAssetEditorViewportToolbar> ViewportToolbar;

	/** Menu extenders */
	TArray<TSharedPtr<FExtender>> Extenders;
	
	/** MyCustomAsset being edited */
	TObjectPtr<UMyCustomAsset> MyCustomAsset;

	TObjectPtr<UStaticMeshComponent> PreviewMeshComponent;

	/** Pointer to the vertical box into which the overlay text items are added */
	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;
};


