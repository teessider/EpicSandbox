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
	virtual ~SMyCustomAssetEditorViewport() override;

	// FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	// End of FGCObject interface

	/** Set the parent tab of the viewport for determining visibility */
	void SetParentTab(TSharedRef<SDockTab> InParentTab) { ParentTab = InParentTab; }

	/** Returns the preview scene being rendered in the viewport */
	TSharedRef<FAdvancedPreviewScene> GetPreviewScene() const { return PreviewScene.ToSharedRef(); }

	static void SetPreviewMesh(
		const TSharedPtr<FAdvancedPreviewScene>& InPreviewScene,
		TObjectPtr<UStaticMeshComponent>& PreviewMeshComponent,
		const TObjectPtr<UStaticMesh>& InStaticMesh);
	void SetPreviewMeshes(const TObjectPtr<UStaticMesh>& FirstInStaticMesh, const TObjectPtr<UStaticMesh>& SecondInStaticMesh);
	void RefreshViewport();
	
	void ResetCamera();

	TObjectPtr<UStaticMeshComponent> GetFirstPreviewMeshComponent() const { return FirstPreviewMeshComponent; }
	TObjectPtr<UStaticMeshComponent> GetSecondPreviewMeshComponent() const { return SecondPreviewMeshComponent; }

protected:
	/** SEditorViewport interface */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual EVisibility OnGetViewportContentVisibility() const override { return IsVisible() ? EVisibility::Visible : EVisibility::Collapsed; }
	virtual void BindCommands() override;
	virtual void OnFocusViewportToSelection() override;
	virtual TSharedPtr<SWidget> BuildViewportToolbar() override;

private:
	/** Determines the visibility of the viewport. */
	virtual bool IsVisible() const override;
	
	void OnObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent);
	
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

	TObjectPtr<UStaticMeshComponent> FirstPreviewMeshComponent;
	TObjectPtr<UStaticMeshComponent> SecondPreviewMeshComponent;

	/** Pointer to the vertical box into which the overlay text items are added */
	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;
};


