// Copyright (c) Andrew Bell

#include "SMyCustomAssetEditorViewportToolbar.h"
#include "SMyCustomAssetEditorViewport.h"

#include "ShowFlagFilter.h"
#include "ShowFlagMenuCommands.h"
#include "SEditorViewportToolBarMenu.h"
#include "EditorViewportCommands.h"
#include "BufferVisualizationMenuCommands.h"
#include "NaniteVisualizationMenuCommands.h"
#include "SEditorViewportViewMenu.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SMyCustomAssetEditorViewportToolbar::Construct(const FArguments& InArgs, TSharedPtr<SMyCustomAssetEditorViewport> InViewport)
{
	// All of this was mostly covered by SCommonEditorViewportToolbarBase::Construct before but GenerateShowMenu is now empty
	// and since that contained Showing the Grid, Bounds & Collision, it was decided to update my code.
	// SAnimViewportToolBar::Construct was used as reference (and the methods it uses like GenerateShowMenu etc.)
	// since it seems like the most modern implementation alongside the main Level Viewport (and it's toolbars) 
	CommandList = InViewport->GetCommandList();
	Extenders = InArgs._Extenders;

	// If there is no extender, there should be one (seems there should always be at least one?)
	if(Extenders.Num() == 0)
	{
		Extenders.Add(MakeShared<FExtender>());
	}

	// Standard Slate code is below
	const FMargin ToolbarSlotPadding(4.0f, 1.0f);

	const TSharedRef<SHorizontalBox> LeftToolbar = SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(ToolbarSlotPadding)
		[
			SNew(SEditorViewportToolbarMenu)
			.ToolTipText(INVTEXT("View Options"))
			.ParentToolBar(SharedThis(this))
			.Cursor(EMouseCursor::Default)
			.Image("EditorViewportToolBar.OptionsDropdown")
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("SMyCustomAssetEditorViewportToolbar.OptionsDropdown")))
			.OnGetMenuContent(this, &SMyCustomAssetEditorViewportToolbar::GenerateViewMenu)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(ToolbarSlotPadding)
		[
			SNew(SEditorViewportToolbarMenu)
			.ToolTipText(INVTEXT("Viewport Options. Use this to switch between different orthographic or perspective views."))
			.ParentToolBar(SharedThis(this))
			.Cursor(EMouseCursor::Default)
			.Label(this, &SMyCustomAssetEditorViewportToolbar::GetCameraMenuLabel)
			.LabelIcon( this, &SMyCustomAssetEditorViewportToolbar::GetCameraMenuLabelIcon)
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("SMyCustomAssetEditorViewportToolbar.CameraViewportTypeMenu")))
			.OnGetMenuContent(this, &SMyCustomAssetEditorViewportToolbar::GenerateViewportTypeMenu)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(ToolbarSlotPadding)
		[
			SNew(SEditorViewportToolbarMenu)
			.ParentToolBar(SharedThis(this))
			.Cursor(EMouseCursor::Default)
			.Label(this, &SMyCustomAssetEditorViewportToolbar::GetBufferViewsMenuLabel)
			.LabelIcon(this, &SMyCustomAssetEditorViewportToolbar::GetBufferViewsMenuLabelIcon)
			.OnGetMenuContent(this, &SMyCustomAssetEditorViewportToolbar::GenerateBufferViewsMenu)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(ToolbarSlotPadding)
		[
			SNew(SEditorViewportToolbarMenu)
			.ToolTipText(INVTEXT("Show Options. Use this enable/disable the rendering of types of scene elements."))
			.ParentToolBar(SharedThis(this))
			.Cursor(EMouseCursor::Default)
			.Label(INVTEXT("Show"))
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("EditorViewportToolBar.ShowMenuButton")))
			.OnGetMenuContent(this, &SMyCustomAssetEditorViewportToolbar::GenerateShowMenu)
		];

	// For every slot above DON'T FORGET TO PUT IT HERE!!!
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("EditorViewportToolBar.Background"))
		.Cursor(EMouseCursor::Default)
		[
			// Since the 3 slots above are all added to the Left Toolbar, only need to add the toolbar here
			LeftToolbar
		]
	];

	SViewportToolBar::Construct(SViewportToolBar::FArguments());

	Viewport = InViewport;
}

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateViewMenu() const
{
	// This menu is "derived" from the one in the main Level viewport (the 3 lines) which has many camera related options
	// (but not the Camera Views menu (has "Perspective" label)) 
	static const FName MenuName("MyCustomAssetEditor.ViewMenu");

	if(!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
		FToolMenuSection& Section = Menu->AddSection("MyCustomAssetViewportCamera", INVTEXT("Camera"));
		// Although Focusing on an object works without this (thanks to SEditorViewport::BindCommands() in SMyCustomAssetEditorViewport connecting it to the input),
		// it's nice to also have a menu option too!
		Section.AddMenuEntry(FEditorViewportCommands::Get().FocusViewportToSelection);
		Section.AddEntry(FToolMenuEntry::InitWidget("FOVAngle", MakeFOVWidget(), INVTEXT("Field of View (H)")));
	}

	TSharedPtr<FExtender> MenuExtender = FExtender::Combine(Extenders);
	FToolMenuContext MenuContext(CommandList, MenuExtender);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

FText SMyCustomAssetEditorViewportToolbar::GetCameraMenuLabel() const
{
	// Same as SLevelViewportToolBar
	TSharedPtr MyCustomAssetEditorViewport(Viewport.Pin());
	if(MyCustomAssetEditorViewport.IsValid())
	{
		return GetCameraMenuLabelFromViewportType(MyCustomAssetEditorViewport->GetViewportClient()->ViewportType);
	}
	return INVTEXT("Camera");
}

const FSlateBrush* SMyCustomAssetEditorViewportToolbar::GetCameraMenuLabelIcon() const
{
	// Same as SLevelViewportToolBar
	TSharedPtr MyCustomAssetEditorViewport(Viewport.Pin());
	if (MyCustomAssetEditorViewport.IsValid())
	{
		return GetCameraMenuLabelIconFromViewportType(MyCustomAssetEditorViewport->GetViewportClient()->ViewportType);
	}
	// Turns out SlateCore has a method for getting "No Brush"
	// different way is in the comment..
	// return FAppStyle::Get().GetBrush("NoBrush");
	return FStyleDefaults::GetNoBrush();
}

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateViewportTypeMenu() const
{
	static const FName MenuName("MyCustomAssetEditor.ViewportTypeMenu");
	
	const FEditorViewportCommands& EditorViewportActions = FEditorViewportCommands::Get();
	
	// This is also how it is in SLevelViewportToolBar::FillCameraMenu 
	if(!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		// This is also how it is in SLevelViewportToolBar::FillCameraMenu without the extra section for placed cameras
		// (after registering the menu in a different method)
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
		{
			FToolMenuSection& Section = Menu->AddSection("CameraTypes");
			Section.AddMenuEntry(EditorViewportActions.Perspective);
		}
		{
			FToolMenuSection& Section = Menu->AddSection("LevelViewportCameraType_Ortho", INVTEXT("Orthographic"));
			Section.AddMenuEntry(EditorViewportActions.Top);
			Section.AddMenuEntry(EditorViewportActions.Bottom);
			Section.AddMenuEntry(EditorViewportActions.Left);
			Section.AddMenuEntry(EditorViewportActions.Right);
			Section.AddMenuEntry(EditorViewportActions.Front);
			Section.AddMenuEntry(EditorViewportActions.Back);
		}
	}

	const TSharedPtr<FExtender> MenuExtender = FExtender::Combine(Extenders);
	const FToolMenuContext MenuContext(CommandList, MenuExtender);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

FText SMyCustomAssetEditorViewportToolbar::GetBufferViewsMenuLabel() const
{
	FText Label = INVTEXT("View");
	
	TSharedPtr<SEditorViewport> EditorViewport = Viewport.Pin();
	if (EditorViewport.IsValid())
	{
		const TSharedPtr<FEditorViewportClient> ViewportClient = EditorViewport->GetViewportClient();
		check(ViewportClient.IsValid());
		const EViewModeIndex ViewMode = ViewportClient->GetViewMode();

		// the View Mode switch here should match what view modes are in GenerateBufferViewsMenu
		// and also be in EViewModeIndex enum
		// (for example, visualising Lumen in the viewport)
		if (ViewMode == VMI_VisualizeBuffer)
		{
			Label = ViewportClient->GetCurrentBufferVisualizationModeDisplayName();
		}
		else if (ViewMode == VMI_VisualizeNanite)
		{
			Label = ViewportClient->GetCurrentNaniteVisualizationModeDisplayName();
		}
		else
		{
			Label = UViewModeUtils::GetViewModeDisplayName(ViewMode);
		}
	}
	
	return Label;
}

const FSlateBrush* SMyCustomAssetEditorViewportToolbar::GetBufferViewsMenuLabelIcon() const
{
	TSharedPtr<SEditorViewport> EditorViewport = Viewport.Pin();
	if (EditorViewport.IsValid())
	{
		const TSharedPtr<FEditorViewportClient> ViewportClient = EditorViewport->GetViewportClient();
		check(ViewportClient.IsValid());
		const EViewModeIndex ViewMode = ViewportClient->GetViewMode();

		return UViewModeUtils::GetViewModeDisplayIcon(ViewMode);
	}

	return FStyleDefaults::GetNoBrush();
}

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateBufferViewsMenu() const
{
	// For this menu...
	// SAnimViewportToolBar actually uses the older style FMenuBuilder to create a menu from FBufferVisualizationMenuCommands BUT
	// looking into SLevelViewportToolBar shows that it is a child of SEditorViewportViewMenu which allows it to redefine GenerateViewMenuContent.
	// This method does the menu generation BUT it is protected and every submenu is locally defined which in the end means it has to be recreated...
	// (unless all this is better exposed in the future...)
	static const FName MenuName("MyCustomAssetEditor.BufferViewsMenu");

	const FEditorViewportCommands& EditorViewportActions = FEditorViewportCommands::Get();
	
	if(!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		// Most of what follows can basically be found in SEditorViewportViewMenu::FillViewMenu
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
		
		FToolMenuSection& Section = Menu->AddSection("ViewMode", INVTEXT("View Mode"));
		{
			// Since this viewport will not likely show different types of Collision (Player vs Visibility),
			// those views have been left out in favour of the Collision Show Flag.
			Section.AddMenuEntry(EditorViewportActions.LitMode, UViewModeUtils::GetViewModeDisplayName(VMI_Lit));
			Section.AddMenuEntry(EditorViewportActions.UnlitMode, UViewModeUtils::GetViewModeDisplayName(VMI_Unlit));
			Section.AddMenuEntry(EditorViewportActions.WireframeMode, UViewModeUtils::GetViewModeDisplayName(VMI_BrushWireframe));
			Section.AddMenuEntry(EditorViewportActions.DetailLightingMode, UViewModeUtils::GetViewModeDisplayName(VMI_Lit_DetailLighting));
			Section.AddMenuEntry(EditorViewportActions.LightingOnlyMode, UViewModeUtils::GetViewModeDisplayName(VMI_LightingOnly));
			Section.AddMenuEntry(EditorViewportActions.ReflectionOverrideMode, UViewModeUtils::GetViewModeDisplayName(VMI_ReflectionOverride));
		}

		// If RayTracing is enabled, it would go here inside a #if RHI_RAYTRACING macro
		// See SEditorViewportViewMenu::FillViewMenu
		// (at the time of writing I don't have a Raytracing capable GPU to see if this would work or not :(
		// (I would very much guess it does since it would be just a copy/paste from Epic ;))
		{
			// In SEditorViewportViewMenu the Optimization Submenu is done inside a local struct with a static function
			// (also the same with other submenus. Maybe it is to not bloat the class?)
			// but the structure/layout is basically the same as other menus. A lot of the debug views are based on platform/shader level support however.
			// Since this is a single viewport with one light source (at least for now) and don't expect to have more, some views will be omitted.
			Section.AddSubMenu(
				"OptimizationSubMenu",
				INVTEXT("Optimization Viewmodes"), INVTEXT("Select optimization visualizer"),
				// the text inside the [] in the Lambda is for capturing local variables to use inside the lambda expression.
				// EditorViewportActions or (Class field) Viewport wouldn't be seen here otherwise!
				FNewToolMenuDelegate::CreateLambda([this, EditorViewportActions](UToolMenu* SubMenu)
				{
					//TODO: FIX Ptr crash
					// const TSharedPtr<SMyCustomAssetEditorViewport> ViewportPtr = Viewport.Pin();
					// const UWorld* World = ViewportPtr->GetViewportClient()->GetWorld();
					
					FToolMenuSection& SubMenuSection = SubMenu->AddSection("OptimizationViewmodes", INVTEXT("Optimization Viewmodes"));
					SubMenuSection.AddMenuEntry(EditorViewportActions.ShaderComplexityMode, UViewModeUtils::GetViewModeDisplayName(VMI_ShaderComplexity));
					// if (AllowDebugViewShaderMode(DVSM_ShaderComplexityContainedQuadOverhead, GMaxRHIShaderPlatform, World->GetFeatureLevel()))
					// {
						SubMenuSection.AddMenuEntry(EditorViewportActions.ShaderComplexityWithQuadOverdrawMode, UViewModeUtils::GetViewModeDisplayName(VMI_ShaderComplexityWithQuadOverdraw));
					// }
					// if (AllowDebugViewShaderMode(DVSM_QuadComplexity, GMaxRHIShaderPlatform, World->GetFeatureLevel()))
					// {
						SubMenuSection.AddMenuEntry(EditorViewportActions.QuadOverdrawMode, UViewModeUtils::GetViewModeDisplayName(VMI_QuadOverdraw));
					// }
				}),
				FUIAction(
					FExecuteAction(),
					FCanExecuteAction(),
					FIsActionChecked::CreateLambda([this]
					{
						//TODO: FIX Ptr crash
						// const TSharedRef<SEditorViewport> EditorViewportRef = Viewport.Pin().ToSharedRef();
						// const TSharedPtr<FEditorViewportClient> EditorViewportClient = EditorViewportRef->GetViewportClient();
						// check(EditorViewportClient.IsValid());
						// 	
						// const EViewModeIndex ViewMode = EditorViewportClient->GetViewMode();
						// return
						// 	ViewMode == VMI_LightComplexity || ViewMode == VMI_ShaderComplexity
						// 	|| ViewMode == VMI_ShaderComplexityWithQuadOverdraw || ViewMode == VMI_QuadOverdraw;
						return false;
					})
				),
				EUserInterfaceActionType::RadioButton,
				/* bInOpenSubMenuOnClick = */ false,
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "EditorViewport.QuadOverdrawMode"));
		}
		{
			// This menu is from SLevelViewportToolBar
			// Visualize Debug Buffer Views
			Section.AddSubMenu(
				"VisualizeVufferViewMode",
				INVTEXT("Buffer Visualization"), INVTEXT("Select a mode for buffer visualization"),
				// When using other "...MenuCommands" DON'T FORGET TO BIND THEM in SMyCustomAssetEditorViewport!!!
				FNewMenuDelegate::CreateStatic(&FBufferVisualizationMenuCommands::BuildVisualisationSubMenu),
				FUIAction(
					FExecuteAction(),
					FCanExecuteAction(),
					FIsActionChecked::CreateLambda([this]
					{
						//TODO: FIX Ptr crash
						// const TSharedRef<SEditorViewport> EditorViewportRef = Viewport.Pin().ToSharedRef();
						// const TSharedPtr<FEditorViewportClient> EditorViewportClient = EditorViewportRef->GetViewportClient();
						// check(EditorViewportClient.IsValid());
						
						// 	return EditorViewportClient->IsViewModeEnabled(VMI_VisualizeBuffer);
						
						return false;
					})
				),
				EUserInterfaceActionType::RadioButton,
				/* bInOpenSubMenuOnClick = */ false,
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "EditorViewport.VisualizeBufferMode")
			);
		}
		{
			// This menu is from SLevelViewportToolBar
			// Visualize Nanite Views
			Section.AddSubMenu(
				"VisualizeNaniteViewMode",
				INVTEXT("Nanite Visualization"),
				INVTEXT("Select a mode for Nanite visualization"),
				// When using other "...MenuCommands" DON'T FORGET TO BIND THEM in SMyCustomAssetEditorViewport!!!
				FNewMenuDelegate::CreateStatic(&FNaniteVisualizationMenuCommands::BuildVisualisationSubMenu),
				FUIAction(
					FExecuteAction(),
					FCanExecuteAction(),
					FIsActionChecked::CreateLambda([this]
					{
						//TODO: FIX Ptr crash
						// const TSharedRef<SEditorViewport> EditorViewportRef = Viewport.Pin().ToSharedRef();
						// const TSharedPtr<FEditorViewportClient> EditorViewportClient = EditorViewportRef->GetViewportClient();
						// check(EditorViewportClient.IsValid());
						//
						// return EditorViewportClient->IsViewModeEnabled(VMI_VisualizeNanite);
						return false;
					})
				),
				EUserInterfaceActionType::RadioButton,
				/* bInOpenSubMenuOnClick = */ false,
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "EditorViewport.VisualizeNaniteMode")
			);
		}
		{
			FToolMenuSection& ExposureMenuSection = Menu->AddSection("Exposure", INVTEXT("Exposure"));

			//TODO: FIX Ptr crash
			// const TSharedRef<SWidget> FixedEV100Menu = Viewport.Pin()->BuildFixedEV100Menu();
			// const TSharedPtr<FEditorViewportClient> EditorViewportClient = Viewport.Pin()->GetViewportClient();
			//
			//
			// const bool bIsLevelEditor = EditorViewportClient.IsValid() && EditorViewportClient->IsLevelEditorClient();
			//
			ExposureMenuSection.AddMenuEntry(EditorViewportActions.ToggleAutoExposure);
			// ExposureMenuSection.AddMenuEntry(bIsLevelEditor ? EditorViewportActions.ToggleInGameExposure : EditorViewportActions.ToggleAutoExposure);
			// ExposureMenuSection.AddEntry(FToolMenuEntry::InitWidget("FixedEV100", FixedEV100Menu, INVTEXT("EV100")));
		}
	}

	TSharedPtr<FExtender> MenuExtender = FExtender::Combine(Extenders);
	FToolMenuContext MenuContext(CommandList, MenuExtender);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}


TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateShowMenu() const
{
	// This below is the new Tools Menus System vs the old Menu Builder
	// It was taken from SAnimViewportToolBar::GenerateShowMenu which also uses FShowFlagFilter & FEngineShowFlags
	// There it is done selectively vs SLevelViewportToolBar which does it more programmatically
	// (makes senses since it is for every Show Flag...)
	static const FName MenuName("MyCustomAssetEditor.ShowMenu");

	if (!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
		Menu->AddDynamicSection("MyCustomAssetSection", FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
		{
			// Only include the show flag filters we need!
			static const FShowFlagFilter ShowFlagFilter = FShowFlagFilter(FShowFlagFilter::ExcludeAllFlagsByDefault)
				// General
				.IncludeFlag(FEngineShowFlags::SF_Collision)
				.IncludeFlag(FEngineShowFlags::SF_Grid)
				// Advanced 
				.IncludeFlag(FEngineShowFlags::SF_Bounds)
				;

			FShowFlagMenuCommands::Get().BuildShowFlagsMenu(InMenu, ShowFlagFilter);
		}));
	}

	TSharedPtr<FExtender> MenuExtender = FExtender::Combine(Extenders);
	FToolMenuContext MenuContext(CommandList, MenuExtender);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

TOptional<float> SMyCustomAssetEditorViewportToolbar::OnGetFOVValue() const
{
	if(Viewport.IsValid())
	{
		return Viewport.Pin()->GetViewportClient()->ViewFOV;
	}
	return 0.0f;
}

void SMyCustomAssetEditorViewportToolbar::OnFOVValueChanged(float NewValue)
{
	TSharedPtr MyCustomAssetEditorViewport(Viewport.Pin());
	if (MyCustomAssetEditorViewport.IsValid())
	{
		TSharedPtr<FEditorViewportClient> EditorViewportClient = MyCustomAssetEditorViewport->GetViewportClient();
		EditorViewportClient->ViewFOV = NewValue;
		EditorViewportClient->Invalidate();
	}
}

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::MakeFOVWidget() const
{
	constexpr float FOVMin = 5.f;
	constexpr float FOVMax = 170.f;

	return
		SNew(SBox)
		.HAlign(HAlign_Right)
		[
			SNew(SBox)
			.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
			.WidthOverride(100.0f)
			[
				SNew(SNumericEntryBox<float>)
				.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
				.AllowSpin(true)
				.MinValue(FOVMin)
				.MaxValue(FOVMax)
				.MinSliderValue(FOVMin)
				.MaxSliderValue(FOVMax)
				.Value(this, &SMyCustomAssetEditorViewportToolbar::OnGetFOVValue)
				.OnValueChanged(const_cast<SMyCustomAssetEditorViewportToolbar*>(this), &SMyCustomAssetEditorViewportToolbar::OnFOVValueChanged)
			]
		];
}
