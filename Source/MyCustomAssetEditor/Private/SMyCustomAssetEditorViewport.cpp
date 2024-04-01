// Copyright (c) Andrew Bell

#include "SMyCustomAssetEditorViewport.h"
#include "SMyCustomAssetEditorViewportClient.h"
#include "SMyCustomAssetEditorViewportToolbar.h"
#include "MyCustomAsset.h"

#include "Slate/SceneViewport.h"
#include "ComponentReregisterContext.h" // For PREVIEW_COMPONENT
#include "ShowFlagMenuCommands.h" // For FShowFlagMenuCommands

void SMyCustomAssetEditorViewport::Construct(const FArguments& InArgs)
{
	PreviewScene->SetFloorVisibility(false, true);
	PreviewScene->SetEnvironmentVisibility(false);
	
	MyCustomAssetEditorPtr = InArgs._MyCustomAssetEditor;
	MyCustomAsset = InArgs._ObjectToEdit;
	Extenders = InArgs._Extenders;
	CurrentViewMode = VMI_Lit;

	// Setup a PREVIEW_COMPONENT here //
	PreviewMeshComponent = NewObject<UStaticMeshComponent>();
	
	// It almost EVERY Editor Viewport uses this method to construct the viewport widget as it sets up the Slate widget in it's entirety!
	// Although one COULD make their version in theory (there are some Viewports which don't use SEditorViewport)
	// At some point in UE5's lifecyle, some extra properties were added (for tracking Slate Widgets?)
	// (Following the example of Skeletal Mesh/Animation Viewport)
	SEditorViewport::Construct(SEditorViewport::FArguments()
		.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
		.AddMetaData<FTagMetaData>(TEXT("MyCustomAssetEditor.Viewport")));

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
	EditorViewportClient = MakeShared<FMyCustomAssetEditorViewportClient>(MyCustomAssetEditorPtr, SharedThis(this), PreviewScene.ToSharedRef());

	EditorViewportClient->bSetListenerPosition = false;

	EditorViewportClient->SetRealtime(true);
	EditorViewportClient->VisibilityDelegate.BindSP(this, &SMyCustomAssetEditorViewport::IsVisible);

	return EditorViewportClient.ToSharedRef();
}

void SMyCustomAssetEditorViewport::BindCommands()
{
	// Multiple Commands can use this CommandList Reference!
	FUICommandList& CommandListRef = *CommandList;

	// All Show Flag Commands are inside of FShowFlagMenuCommands so only need to call that BindCommands method here!
	FShowFlagMenuCommands::Get().BindCommands(CommandListRef, Client);

	// All the commands listed here should mirror what is in FMyCustomAssetEditorCommands::RegisterCommands!!
	// These are unique to this viewport/toolkit

}

void SMyCustomAssetEditorViewport::OnFocusViewportToSelection()
{
	SEditorViewport::OnFocusViewportToSelection();
}

TSharedPtr<SWidget> SMyCustomAssetEditorViewport::MakeViewportToolbar()
{
	// Since this is used as a variable later, SAssignNew is used rather than SNew
	return SAssignNew(ViewportToolbar, SMyCustomAssetEditorViewportToolbar, SharedThis(this))
	.Visibility(EVisibility::SelfHitTestInvisible)
	.Cursor(EMouseCursor::Default)
	.Extenders(Extenders);
}

bool SMyCustomAssetEditorViewport::IsVisible() const
{
	// all viewports do something very similar
	return ViewportWidget.IsValid() && (!ParentTab.IsValid() || ParentTab.Pin()->IsForeground()) && SEditorViewport::IsVisible();
}
