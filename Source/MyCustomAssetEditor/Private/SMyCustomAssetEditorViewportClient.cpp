// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCustomAssetEditorViewportClient.h"

#include "AssetViewerSettings.h"
#include "Editor/EditorPerProjectUserSettings.h"


FMyCustomAssetEditorViewportClient::FMyCustomAssetEditorViewportClient(TWeakPtr<FMyCustomAssetEditor> InMyCustomAssetEditor, const TSharedRef<SMyCustomAssetEditorViewport>& InMyCustomAssetEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene, UMyCustomAsset* InPreviewMyCustomAsset)
	: FEditorViewportClient(nullptr, &InPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(InMyCustomAssetEditorViewport))
	, MyCustomAssetEditorPtr(InMyCustomAssetEditor)
	, MyCustomAssetEditorViewportPtr(InMyCustomAssetEditorViewport)
{
	// Setup defaults for the common draw helper. 
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = false;
	DrawHelper.GridColorAxis = FColor(160,160,160);
	DrawHelper.GridColorMajor = FColor(144,144,144);
	DrawHelper.GridColorMinor = FColor(128,128,128);
	DrawHelper.PerspectiveGridSize = 2048.0f;
	DrawHelper.NumCells = DrawHelper.PerspectiveGridSize / (16 /**Cellsize*/ * 2);

	// Rider suggests to explicitly call the function even though this is usually not called so explicitly elsewhere in the engine...
	FEditorViewportClient::SetViewMode(VMI_Lit);

	EngineShowFlags.SetSeparateTranslucency(true);
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.SetCompositeEditorPrimitives(true);
	OverrideNearClipPlane(1.0f);
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

bool FMyCustomAssetEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	// Same as Static Mesh Viewport Client. Lidar Point Cloud one is more complex due to the different "modes" (Edit, Selection Modes)
	// otherwise it is the same.
	
	bool bHandled = FEditorViewportClient::InputKey(InViewport, ControllerId, Key, Event, AmountDepressed, false);

	// Handle viewport screenshot.
	bHandled |= InputTakeScreenshot( InViewport, Key, Event );

	bHandled |= AdvancedPreviewScene->HandleInputKey(InViewport, ControllerId, Key, Event, AmountDepressed, bGamepad);

	return bHandled;
}

bool FMyCustomAssetEditorViewportClient::InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	// Exactly like Static Mesh, Material Editor & LidarPoint Cloud (with a small selection exception there) Viewport clients
	bool bResult = true;
	
	if (!bDisableInput)
	{
		bResult = AdvancedPreviewScene->HandleViewportInput(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
		if (bResult)
		{
			Invalidate();
		}
		else
		{
			bResult = FEditorViewportClient::InputAxis(InViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
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
