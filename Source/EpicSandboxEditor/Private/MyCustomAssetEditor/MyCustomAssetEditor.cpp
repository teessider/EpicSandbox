// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomAssetEditor.h"

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

const FName FMyCustomAssetEditor::CustomAssetEditorAppIdentifier(TEXT("CustomAssetEditorApp"));

const FName FMyCustomAssetEditor::DetailsTabId(TEXT("CustomAssetEditor_Details"));
const FName FMyCustomAssetEditor::ViewportTabId(TEXT("CustomAssetEditor_Viewport"));

// TODO: GET INITIAL ASSET EDITOR RUNNING

// CURRENT PROGRESS: Got it to compile so far(with just the Main Asset Editor Toolbar) but it is not called by anything...
// will have to do that via the AssetTypeActions class...
void FMyCustomAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// VIP - Adds the Custom Editor Tab/Window "group" to the local Workspace - 
	// A Core Slate thing to do (every editor tab/window does this when Registering Tab Spawners for the TabManager
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CustomAssetEditor", "Custom Asset Editor"));

	// For each tab within the editor window/group, it needs to be registered with the TabManager
	// The one from FAssetEditorToolkit has the Main Toolbar built-in hence why it is always done first
	// (and finding the workspace root and base Asset Editor Tab category which is VERY IMPORTANT)
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Now for the rest of the editor window/group tabs to be registered with the TabManager
	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("PropertiesTabLabel", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	// InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditor::SpawnTab_Viewport))
	// 	.SetDisplayName(LOCTEXT("ViewportTabLabel", "Viewport"))
	// 	.SetGroup(WorkspaceMenuCategory.ToSharedRef())
	// 	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
		
}

void FMyCustomAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// This one unregisters the Main Toolbar
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	// For every spawned tab...don't forget to unregister it ;)
	InTabManager->UnregisterTabSpawner(DetailsTabId);
	InTabManager->UnregisterTabSpawner(ViewportTabId);

	// InTabManager->UnregisterAllTabSpawners();  // would this do the same as above ?
}

void FMyCustomAssetEditor::InitMyCustomAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit)
{
	// For having a Details View/panel, the Property Editor Module is needed and that has a handy method! (CreateDetailView)
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>( "PropertyEditor" );

	// The following block does the same as the constructor below...but it is more explicitly done here for my learning benefit ;)
	// Also some editors do it like this...and some do it like below? (only big difference is that one is const and the other isn't...)
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ObjectsUseNameArea;
	DetailsViewArgs.bHideSelectionTip = false;

	// const FDetailsViewArgs DetailsViewArgs(
	// 	/*bIsUpdatable*/false,
	// 	/*bIsLockable*/false,
	// 	/*bAllowSearch*/true,
	// 	FDetailsViewArgs::ObjectsUseNameArea,
	// 	/*bHideSelectionTip*/false );
	
	DetailsView = PropertyEditorModule.CreateDetailView( DetailsViewArgs );

	// TODO: EXPLAIN
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CustomAssetEditor_Layout_v0")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->SetHideTabWell( true )
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter()
			->Split
			(
				FTabManager::NewStack()
				->AddTab(DetailsTabId, ETabState::OpenedTab)
			)
		)
	);
	
	// This NEEDs to be called so that the default layout, menu and toolbar are created.
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost,	CustomAssetEditorAppIdentifier,	StandaloneDefaultLayout,/*bCreateDefaultStandaloneMenu*/ true,/*bCreateDefaultToolbar*/ true, ObjectsToEdit);

	RegenerateMenusAndToolbars();
}

FMyCustomAssetEditor::~FMyCustomAssetEditor()
{
	DetailsView.Reset();
}

FName FMyCustomAssetEditor::GetToolkitFName() const
{
	return FName("CustomAssetEditor");
}

FText FMyCustomAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Asset Editor");
}

FString FMyCustomAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Custom Asset ").ToString();
}

FLinearColor FMyCustomAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Green;
}

void FMyCustomAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(MyCustomAsset);
}

// Basically a copy of SimpleAssetEditor::SpawnPropertiesTab method for now
TSharedRef<SDockTab> FMyCustomAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	// Should ALWAYS check if the TabType is the correct TabId String
	check(Args.GetTabId().TabType == DetailsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("GenericDetailsTitle", "Details"))
		.TabColorScale(GetTabColorScale())
		[
			DetailsView.ToSharedRef()
		];
}

// TSharedRef<SDockTab> FMyCustomAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args) const
// {
// 	check(Args.GetTabId().TabType == ViewportTabId);
//
// 	TSharedRef<SDockTab> SpawnedTab =
// 		SNew(SDockTab)
// 		.Label(LOCTEXT("ViewportTab_Title", "Viewport"))
// 		[
// 			Viewport.ToSharedRef()
// 		];
// 	
// 	Viewport->SetParentTab(SpawnedTab);
// 	
// 	return SpawnedTab;
// }
#undef LOCTEXT_NAMESPACE
