// Copyright (c) Andrew Bell


#include "MyCustomAssetEditor.h"
#include "SMyCustomAssetEditorViewport.h"

#include "AdvancedPreviewSceneModule.h"

#include "MyCustomAsset.h"

const FName FMyCustomAssetEditor::CustomAssetEditorAppIdentifier(TEXT("MyCustomAssetEditorApp"));

const FName FMyCustomAssetEditor::DetailsTabId(TEXT("MyCustomAssetEditor_Details"));
const FName FMyCustomAssetEditor::ViewportTabId(TEXT("MyCustomAssetEditor_Viewport"));
const FName FMyCustomAssetEditor::AdvancedPreviewSettingsTabId(TEXT("MyCustomAssetEditor_AdvancedPreviewSettings"));

void FMyCustomAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// VIP - Adds the Custom Editor Tab/Window "group" to the local Workspace - 
	// A Core Slate thing to do (every editor tab/window does this when Registering Tab Spawners for the TabManager
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("My Custom Asset Editor"));

	// For each tab within the editor window/group, it needs to be registered with the TabManager
	// The one from FAssetEditorToolkit has the Main Toolbar built-in hence why it is always done first
	// (and finding the workspace root and base Asset Editor Tab category which is VERY IMPORTANT)
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Now for the rest of the editor window/group tabs to be registered with the TabManager
	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditor::SpawnTab_Details))
		.SetDisplayName(INVTEXT("Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
	
	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditor::SpawnTab_Viewport))
		.SetDisplayName(INVTEXT("Viewport"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"));
		
	InTabManager->RegisterTabSpawner(AdvancedPreviewSettingsTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditor::SpawnTab_AdvancedPreviewSettings))
		.SetDisplayName(INVTEXT("Preview Scene Settings"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FMyCustomAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// This one unregisters the Main Toolbar
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	
	// For every spawned tab...don't forget to unregister it ;)
	InTabManager->UnregisterTabSpawner(DetailsTabId);
	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(AdvancedPreviewSettingsTabId);

	// InTabManager->UnregisterAllTabSpawners();  // This would this do the same as above in one go?
}

void FMyCustomAssetEditor::InitMyCustomAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, TObjectPtr<UMyCustomAsset> InMyCustomAsset)
{
	MyCustomAssetBeingEdited = InMyCustomAsset;
	
	// For having a Details View/panel, the PropertyEditor Module is used and it has a handy method! (CreateDetailView)
	// Without using the PropertyEditor Module, the way to make a details tab seems to be doing some kind of SSingleObjectDetailsPanel (as seen in LidarPointCloudEditor Module).
	// The way of using the PropertyEditor Module seems to be much more convenient to use. If more is needed then a Details Customization would also be used (as seen in StaticMeshEditor Module)
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>( "PropertyEditor" );

	// The following block does the same as the constructor below...but it is more explicitly done here for my learning benefit ;)
	// Also some editors do it like this...and some do it like below? (only big difference here is that one is const and the other isn't...)
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
	DetailsView->SetObject(InMyCustomAsset);  // Don't forget this! Otherwise the details tab will be EMPTY ;)

	TSharedPtr<FMyCustomAssetEditor> MyCustomAssetEditor = SharedThis(this); 
	
	Viewport = SNew(SMyCustomAssetEditorViewport).MyCustomAssetEditor(SharedThis(this)).ObjectToEdit(MyCustomAssetBeingEdited);

	// TODO: EXPLAIN
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_MyCustomAssetEditor_Layout_v2.1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->SetSizeCoefficient(0.9f)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f) // for each split, each size coefficient should equal to the sum of 1.0!
				->AddTab(ViewportTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab(DetailsTabId, ETabState::OpenedTab)
				->AddTab(AdvancedPreviewSettingsTabId, ETabState::ClosedTab)
				->SetForegroundTab(DetailsTabId)
			)
		)
	);
	
	// This NEEDs to be called so that the default layout, menu and toolbar are created.
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	InitAssetEditor(Mode, InitToolkitHost, CustomAssetEditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InMyCustomAsset);

	RegenerateMenusAndToolbars();
}

FMyCustomAssetEditor::~FMyCustomAssetEditor()
{
	DetailsView.Reset();
}

FName FMyCustomAssetEditor::GetToolkitFName() const
{
	return FName("MyCustomAssetEditor");
}

FText FMyCustomAssetEditor::GetBaseToolkitName() const
{
	return INVTEXT("My Custom Asset Editor");
}

FString FMyCustomAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("My Custom Asset ");
}

FLinearColor FMyCustomAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Green;
}

void FMyCustomAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(MyCustomAssetBeingEdited);
}

FString FMyCustomAssetEditor::GetReferencerName() const
{
	return TEXT("FMyCustomAssetEditor");
}

// Basically a copy of SimpleAssetEditor::SpawnPropertiesTab method.
// If using the DetailsView from IDetailsView, this seems to be the method to use. This is also done in the StaticMeshEditor Module (the SpawnTab_Properties method)
TSharedRef<SDockTab> FMyCustomAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	// Should ALWAYS check if the TabType is the correct TabId String
	check(Args.GetTabId().TabType == DetailsTabId);

	return SNew(SDockTab)
		.Label(INVTEXT("Details"))
		.TabColorScale(GetTabColorScale())
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FMyCustomAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId().TabType == ViewportTabId);

	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.Label(INVTEXT("Viewport"))
		[
			Viewport.ToSharedRef()
		];
	
	Viewport->SetParentTab(SpawnedTab);
	
	return SpawnedTab;
}

TSharedRef<SDockTab> FMyCustomAssetEditor::SpawnTab_AdvancedPreviewSettings(const FSpawnTabArgs& SpawnTabArgs) const
{
	check(SpawnTabArgs.GetTabId().TabType == AdvancedPreviewSettingsTabId);

	TSharedRef<SWidget> InWidget = SNullWidget::NullWidget;
	if (Viewport.IsValid())
	{
		FAdvancedPreviewSceneModule& AdvancedPreviewSceneModule = FModuleManager::LoadModuleChecked<FAdvancedPreviewSceneModule>("AdvancedPreviewScene");
		InWidget = AdvancedPreviewSceneModule.CreateAdvancedPreviewSceneSettingsWidget(Viewport->GetPreviewScene());
	}

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(INVTEXT("Preview Scene Settings"))
		[
			SNew(SBox)
			[
				InWidget
			]
		];

	return SpawnedTab;
}
