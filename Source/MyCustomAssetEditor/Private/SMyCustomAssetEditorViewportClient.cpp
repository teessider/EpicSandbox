// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCustomAssetEditorViewportClient.h"

#include "AssetViewerSettings.h"
#include "Editor/EditorPerProjectUserSettings.h"


FMyCustomAssetEditorViewportClient::FMyCustomAssetEditorViewportClient(TWeakPtr<FMyCustomAssetEditor> InMyCustomAssetEditor, const TSharedRef<SMyCustomAssetEditorViewport>& InMyCustomAssetEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene)
	: FEditorViewportClient(nullptr, &InPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(InMyCustomAssetEditorViewport))
	, MyCustomAssetEditorPtr(InMyCustomAssetEditor)
	, MyCustomAssetEditorViewportPtr(InMyCustomAssetEditorViewport)
{
	// Setup defaults for the common draw helper.
	// Toggling the grid now relies on the Show Flag (SF_GRID)
	// so it seems there is not a need to define the grid here unless something really custom is necessary
	DrawHelper.bDrawGrid = true;
	DrawHelper.PerspectiveGridSize = UE_OLD_HALF_WORLD_MAX1;

	EngineShowFlags.SetSeparateTranslucency(true);
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	OverrideNearClipPlane(UE_KINDA_SMALL_NUMBER);  // It is expected that the meshes in this Viewport can be quite small
	bUsingOrbitCamera = true;
	
	// This is how it is done in the Static Mesh & LidarPointCloud preview scenes
	// unless ModeTools is enabled (which is the case with the Skeletal Mesh Editor Preview Scene("Persona")).
	// That method seems to be a lot more common in the engine though
	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);

	// Register delegate to update the show flags when the post processing is turned on or off
	UAssetViewerSettings::Get()->OnAssetViewerSettingsChanged().AddRaw(this, &FMyCustomAssetEditorViewportClient::OnAssetViewerSettingsChanged);

	// Set correct flags according to current profile settings
	SetAdvancedShowFlagsForScene(UAssetViewerSettings::Get()->Profiles[GetMutableDefault<UEditorPerProjectUserSettings>()->AssetViewerProfileIndex].bPostProcessingEnabled);
}

FMyCustomAssetEditorViewportClient::~FMyCustomAssetEditorViewportClient()
{
	// Be sure to remove the delegate!
	UAssetViewerSettings::Get()->OnAssetViewerSettingsChanged().RemoveAll(this);
}

void FMyCustomAssetEditorViewportClient::MouseMove(FViewport* InViewport, int32 X, int32 Y)
{
	FEditorViewportClient::MouseMove(InViewport, X, Y);
}

bool FMyCustomAssetEditorViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	// Same as Static Mesh Viewport Client. Lidar Point Cloud one is more complex due to the different "modes" (Edit, Selection Modes)
	// otherwise it is the same.
	
	bool bHandled = FEditorViewportClient::InputKey(EventArgs);

	// Handle viewport screenshot.
	bHandled |= InputTakeScreenshot( EventArgs.Viewport, EventArgs.Key, EventArgs.Event );

	bHandled |= AdvancedPreviewScene->HandleInputKey(EventArgs);

	return bHandled;
}

bool FMyCustomAssetEditorViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	// Exactly like Static Mesh, Material Editor & LidarPoint Cloud (with a small selection exception there) Viewport clients
	bool bResult = true;
	
	if (!bDisableInput)
	{
		bResult = AdvancedPreviewScene->HandleViewportInput(InViewport, DeviceId, Key, Delta, DeltaTime, NumSamples, bGamepad);
		if (bResult)
		{
			Invalidate();
		}
		else
		{
			bResult = FEditorViewportClient::InputAxis(InViewport, DeviceId, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}

	return bResult;
}

void FMyCustomAssetEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	// In Static Mesh version, this is used for selecting things like the Sockets, Collision Prims, Edges etc.
}

void FMyCustomAssetEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

void FMyCustomAssetEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);
}

void FMyCustomAssetEditorViewportClient::DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas)
{
	FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
}

bool FMyCustomAssetEditorViewportClient::ShouldOrbitCamera() const
{
	if (GetDefault<ULevelEditorViewportSettings>()->bUseUE3OrbitControls)
	{
		// this editor orbits always if ue3 orbit controls are enabled
		return true;
	}

	return FEditorViewportClient::ShouldOrbitCamera();
}

void FMyCustomAssetEditorViewportClient::ResetCamera()
{
}

void FMyCustomAssetEditorViewportClient::PerspectiveCameraMoved()
{
	// Same as Lidar Point Cloud & Static Mesh Viewport Clients (except Static Mesh has more code for storing the camera position...)
	
	FEditorViewportClient::PerspectiveCameraMoved();

	// If in the process of transitioning to a new location, don't update the orbit camera position.
	// On the final update of the transition, we will get here with IsPlaying()==false, and the editor camera position will
	// be correctly updated.
	if (GetViewTransform().IsPlaying())
	{
		return;
	}

	ToggleOrbitCamera(bUsingOrbitCamera);
}

void FMyCustomAssetEditorViewportClient::OnAssetViewerSettingsChanged(const FName& InPropertyName)
{
	// The following purely checks if post-processing is enabled for the preview scene.
	// (there can be multiple profiles stored globally (local or shared) hence the ProfileIndex stuff)
	if (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bPostProcessingEnabled) || InPropertyName == NAME_Name)
	{
		UAssetViewerSettings* Settings = UAssetViewerSettings::Get();
		const int32 ProfileIndex = AdvancedPreviewScene->GetCurrentProfileIndex();
		if (Settings->Profiles.IsValidIndex(ProfileIndex))
		{
			SetAdvancedShowFlagsForScene(Settings->Profiles[ProfileIndex].bPostProcessingEnabled);
		}
	}
	
}

void FMyCustomAssetEditorViewportClient::SetAdvancedShowFlagsForScene(const bool bAdvancedShowFlags)
{
	if (bAdvancedShowFlags)
	{
		EngineShowFlags.EnableAdvancedFeatures();
	}
	else
	{
		EngineShowFlags.DisableAdvancedFeatures();
	}
}
