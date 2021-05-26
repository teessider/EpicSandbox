// Fill out your copyright notice in the Description page of Project Settings.

#include "EpicSandboxEditorStyle.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FEpicSandboxEditorStyle::EditorStyle = nullptr;

#define IMAGE_BRUSH(RelativePath, ... ) FSlateImageBrush(EditorStyle->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

void FEpicSandboxEditorStyle::Initialize()
{
	if (EditorStyle.IsValid())
	{
		return;
	}
	
	// EditorStyle = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	EditorStyle = MakeShared<FSlateStyleSet>(GetStyleSetName());

	// For now though the default one(s) are used (the editor folders)
	//F:\SandboxEngine\Engine\Content\Editor\Slate\Icons\AssetIcons <- location of editor icons
	EditorStyle->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	EditorStyle->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	// Class Icons are 16x16
	// Class Thumbnails are 64x64
	// 
	// Toolbar Small Icons are 20x20
	// Toolbar Icons are 40x40

	// MyCustomAsset style
	EditorStyle->Set("ClassIcon.MyCustomAsset",	new IMAGE_BRUSH("Icons/AssetIcons/DataAsset_16x", Icon16x16));
	EditorStyle->Set("ClassThumbnail.MyCustomAsset", new IMAGE_BRUSH("Icons/AssetIcons/DataAsset_64x", Icon64x64));
	// EditorStyle->Set("AssetColor.MyCustomAsset", FColor::Emerald); // Could define this here as part of the style but would need a TSharedRef to the ISlateStyleSet version of EditorStyle
			
	FSlateStyleRegistry::RegisterSlateStyle(*EditorStyle.Get()); // Make sure to register the style!
}

void FEpicSandboxEditorStyle::Shutdown()
{
	// UnRegisterSlateStyle has a IsValid() check so it does not need to done separately (surrounding everything in a IsValid() if statement)
	FSlateStyleRegistry::UnRegisterSlateStyle(EditorStyle->GetStyleSetName());
	ensure(EditorStyle.IsUnique());
	EditorStyle.Reset();  // if nothing points to this then this also destroys the pointer too :)
}

FName FEpicSandboxEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EpicSandboxEditorStyle"));
	return StyleSetName;
}
