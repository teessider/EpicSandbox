// Copyright (c) Andrew Bell

#include "SMyCustomAssetEditorViewportToolbar.h"
#include "SMyCustomAssetEditorViewport.h"

#include "ShowFlagFilter.h"
#include "ShowFlagMenuCommands.h"
#include "SEditorViewportToolBarMenu.h"
#include "EditorViewportCommands.h"
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
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("EditorViewportToolBar.MenuDropdown")))
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
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("EditorViewportToolBar.CameraMenu")))
			.OnGetMenuContent(this, &SMyCustomAssetEditorViewportToolbar::GenerateViewportTypeMenu)
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
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("ViewMenuButton")))
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
	
	// This is also how it is in SLevelViewportToolBar::FillCameraMenu 
	if(!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		// This is also how it is in SLevelViewportToolBar::FillCameraMenu without the extra section for placed cameras
		// (after registering the menu in a different method)
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);
		{
			FToolMenuSection& Section = Menu->AddSection("CameraTypes");
			Section.AddMenuEntry(FEditorViewportCommands::Get().Perspective);
		}
		{
			FToolMenuSection& Section = Menu->AddSection("LevelViewportCameraType_Ortho", INVTEXT("Orthographic"));
			Section.AddMenuEntry(FEditorViewportCommands::Get().Top);
			Section.AddMenuEntry(FEditorViewportCommands::Get().Bottom);
			Section.AddMenuEntry(FEditorViewportCommands::Get().Left);
			Section.AddMenuEntry(FEditorViewportCommands::Get().Right);
			Section.AddMenuEntry(FEditorViewportCommands::Get().Front);
			Section.AddMenuEntry(FEditorViewportCommands::Get().Back);
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
