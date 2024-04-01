// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"

#include "SMyCustomAssetEditorViewport.h"

class FAdvancedPreviewScene;

class FMyCustomAssetEditor;
class SMyCustomAssetEditorViewport;
class UMyCustomAsset;


class FMyCustomAssetEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FMyCustomAssetEditorViewportClient>
{
public:
	FMyCustomAssetEditorViewportClient(TWeakPtr<FMyCustomAssetEditor> InMyCustomAssetEditor, const TSharedRef<SMyCustomAssetEditorViewport>& InMyCustomAssetEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene);
	virtual ~FMyCustomAssetEditorViewportClient() override;

	// FEditorViewportClient interface
	virtual void MouseMove(FViewport* Viewport, int32 x, int32 y) override;
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	virtual bool InputAxis(FViewport* Viewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
	virtual void ProcessClick(class FSceneView& View, class HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;
	virtual bool InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale) override { return false; }
	virtual void TrackingStarted(const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge) override {}
	virtual void TrackingStopped() override {}
	virtual UE::Widget::EWidgetMode GetWidgetMode() const override { return UE::Widget::WM_None; }
	virtual void SetWidgetMode(UE::Widget::EWidgetMode NewMode) override {}
	virtual bool CanSetWidgetMode(UE::Widget::EWidgetMode NewMode) const override { return false; }
	virtual bool CanCycleWidgetMode() const override { return false; }
	virtual FVector GetWidgetLocation() const override { return FVector::ZeroVector; }
	virtual FMatrix GetWidgetCoordSystem() const override { return FMatrix::Identity; }
	virtual ECoordSystem GetWidgetCoordSystemSpace() const override { return COORD_Local; }
	virtual bool ShouldOrbitCamera() const override;
	// End of FEditorViewportClient interface

	void ResetCamera();
	
	/** Called when viewport focuses on a selection */
	void OnFocusViewportToSelection();
	
protected:
	// FEditorViewportClient interface
	virtual void PerspectiveCameraMoved() override;

	/** Call back for when the user changes preview scene settings in the UI */
	void OnAssetViewerSettingsChanged(const FName& InPropertyName);
	/** Used to (re)-set the viewport show flags related to post processing*/
	void SetAdvancedShowFlagsForScene(const bool bAdvancedShowFlags);

private:
	UE::Widget::EWidgetMode WidgetMode;
	
	/** Pointer back to the MyCustomAsset editor tool that owns us */
	TWeakPtr<FMyCustomAssetEditor> MyCustomAssetEditorPtr;
	
	/** Pointer back to the MyCustomAssetEditor viewport control that owns us */
	TWeakPtr<SMyCustomAssetEditorViewport> MyCustomAssetEditorViewportPtr;

	/** Stored pointer to the preview scene in which the MyCustomAsset mesh(es) is/are shown */
	FAdvancedPreviewScene* AdvancedPreviewScene;
};
