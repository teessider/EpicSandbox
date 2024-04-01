// Fill out your copyright notice in the Description page of Project Settings.

#include "SMyCustomAssetEditorViewportToolbar.h"
#include "SMyCustomAssetEditorViewport.h"
#include "ShowFlagFilter.h"
#include "ShowFlagMenuCommands.h"
#include "SEditorViewportToolBarMenu.h"

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

TSharedRef<SWidget> SMyCustomAssetEditorViewportToolbar::GenerateShowMenu() const
{
	// This below is the new Tools Menus System vs the old Menu Builder
	// It was taken from SAnimViewportToolBar::GenerateShowMenu which also uses FShowFlagFilter & FEngineShowFlags
	// There it is done selectively vs SLevelViewportToolBar which does it more programmatically
	// (makes senses since it is for every Show Flag...)
	static const FName MenuName("MyCustomAssetEditor.ViewportToolbar");

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
