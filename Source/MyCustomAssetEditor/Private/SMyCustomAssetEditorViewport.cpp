// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCustomAssetEditorViewport.h"
#include "SMyCustomAssetEditorViewportClient.h"
#include "SMyCustomAssetEditorViewportToolbar.h"
#include "MyCustomAssetEditorCommands.h"
#include "MyCustomAsset.h"

#include "Slate/SceneViewport.h"
#include "ComponentReregisterContext.h" // For PREVIEW_COMPONENT

void SMyCustomAssetEditorViewport::Construct(const FArguments& InArgs)
{
	PreviewScene->SetFloorVisibility(false, true);
	PreviewScene->SetEnvironmentVisibility(false);
	
	MyCustomAssetEditorPtr = InArgs._MyCustomAssetEditor;
	MyCustomAsset = InArgs._ObjectToEdit;
	CurrentViewMode = VMI_Lit;

	// Setup a PREVIEW_COMPONENT here //
	PreviewMeshComponent = NewObject<UStaticMeshComponent>();
	
	// It almost EVERY Editor Viewport uses this method to construct the viewport widget as it sets up the Slate widget in it's entirety!
	// Although one COULD make their version in theory (there are some Viewports which don't use SEditorViewport)
	SEditorViewport::Construct(SEditorViewport::FArguments());

	// Reregister Component here //
	FComponentReregisterContext ReregisterContext(PreviewMeshComponent);	

	// SET_PREVIEW_COMPONENT_MESH(MyCustomAsset);
	PreviewScene->AddComponent(PreviewMeshComponent, FTransform::Identity);
	PreviewMeshComponent->SetStaticMesh(MyCustomAsset->FirstCustomStaticMesh);
	PreviewMeshComponent->MarkRenderStateDirty();

	ResetCamera();

	ViewportOverlay->AddSlot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	.Padding(FMargin(10.0f, 40.0f, 10.0f, 10.0f))
	[
		SAssignNew(OverlayTextVerticalBox, SVerticalBox)
	];
	
}

SMyCustomAssetEditorViewport::SMyCustomAssetEditorViewport()
	: PreviewScene(MakeShared<FAdvancedPreviewScene>(FPreviewScene::ConstructionValues()))
{
	
}

void SMyCustomAssetEditorViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(MyCustomAsset);
	Collector.AddReferencedObject(PreviewMeshComponent);
}

FString SMyCustomAssetEditorViewport::GetReferencerName() const
{
	return TEXT("SMyCustomAssetEditorViewport");
}

TSharedRef<SEditorViewport> SMyCustomAssetEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SMyCustomAssetEditorViewport::GetExtenders() const
{
	//TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	TSharedPtr<FExtender> Result(MakeShared<FExtender>());
	return Result;
}

void SMyCustomAssetEditorViewport::OnFloatingButtonClicked()
{
	// Except for the Level Editor viewport, this is EMPTY
}

void SMyCustomAssetEditorViewport::RefreshViewport()
{
	// Invalidate the viewport's display.
	SceneViewport->Invalidate();
}

void SMyCustomAssetEditorViewport::ResetCamera()
{
	if (FMyCustomAssetEditorViewportClient* EditorViewportClientRawPtr = EditorViewportClient.Get())
	{
		EditorViewportClientRawPtr->ResetCamera();
	}
}

TSharedRef<FEditorViewportClient> SMyCustomAssetEditorViewport::MakeEditorViewportClient()
{
	// EditorViewportClient = MakeShareable(new FMyCustomAssetEditorViewportClient(MyCustomAssetEditorPtr, SharedThis(this), &PreviewScene, MyCustomAsset));
	EditorViewportClient = MakeShared<FMyCustomAssetEditorViewportClient>(MyCustomAssetEditorPtr, SharedThis(this), PreviewScene.ToSharedRef(), MyCustomAsset);

	EditorViewportClient->bSetListenerPosition = false;

	EditorViewportClient->SetRealtime(true);
	EditorViewportClient->VisibilityDelegate.BindSP(this, &SMyCustomAssetEditorViewport::IsVisible);

	return EditorViewportClient.ToSharedRef();
}

void SMyCustomAssetEditorViewport::BindCommands()
{
	// Inside of SEditorViewport::BindCommands()
	SEditorViewport::BindCommands();

	const FMyCustomAssetEditorCommands& Commands = FMyCustomAssetEditorCommands::Get();

	const TSharedRef<FMyCustomAssetEditorViewportClient> EditorViewportClientRef = EditorViewportClient.ToSharedRef();

	CommandList->MapAction(
		Commands.SetShowGrid,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::SetShowGrid),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::IsSetShowGridChecked));

	CommandList->MapAction(
		Commands.SetShowBounds,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::ToggleShowBounds),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::IsSetShowBoundsChecked));

	CommandList->MapAction(
		Commands.SetShowCollision,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::SetShowCollision),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FMyCustomAssetEditorViewportClient::IsSetShowCollisionChecked));
}

void SMyCustomAssetEditorViewport::OnFocusViewportToSelection()
{
	SEditorViewport::OnFocusViewportToSelection();
}

TSharedPtr<SWidget> SMyCustomAssetEditorViewport::MakeViewportToolbar()
{
	return SNew(SMyCustomAssetEditorViewportToolbar, SharedThis(this));
}

bool SMyCustomAssetEditorViewport::IsVisible() const
{
	// all viewports do something very similar
	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground()) && SEditorViewport::IsVisible();
}
