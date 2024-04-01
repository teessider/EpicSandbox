// Copyright (c) Andrew Bell

#include "EpicSandboxEditorStyle.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"

// Used as part of the IMAGE_BRUSH & IMAGE_BRUSH_SVG Macros from SlateStyleMacros!
// Hence why this is here as opposed to basically duplicating the existing macros...
#define RootToContentDir EditorStyle->RootToContentDir

TSharedPtr<FSlateStyleSet> FEpicSandboxEditorStyle::EditorStyle = nullptr;

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
	//F:\SandboxEngine\Engine\Content\Editor\Slate\Starship\AssetIcons <- location of new SVG icons
	EditorStyle->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	EditorStyle->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	// Class Icons are 16x16
	// Class Thumbnails are 64x64
	// 
	// Toolbar Icons are 20x20

	// MyCustomAsset style
	EditorStyle->Set("ClassIcon.MyCustomAsset",	new IMAGE_BRUSH_SVG("Starship/AssetIcons/DataAsset_16", Icon16x16));
	EditorStyle->Set("ClassThumbnail.MyCustomAsset", new IMAGE_BRUSH_SVG("Starship/AssetIcons/DataAsset_64", Icon64x64));
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
