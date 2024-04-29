// Copyright (c) Andrew Bell


#include "AssetDefinition_MyCustomAsset.h"
#include "MyCustomAsset.h"
#include "EpicSandboxEditor.h"
#include "MyCustomAssetEditor.h"

FText UAssetDefinition_MyCustomAsset::GetAssetDisplayName() const
{
	return INVTEXT("Custom Asset");
}

FLinearColor UAssetDefinition_MyCustomAsset::GetAssetColor() const
{
	return FLinearColor(FColor::Emerald);
}

TSoftClassPtr<> UAssetDefinition_MyCustomAsset::GetAssetClass() const
{
	return UMyCustomAsset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_MyCustomAsset::GetAssetCategories() const
{
	static const auto Categories =
		{
			FAssetCategoryPath(FEpicSandboxEditor::GetAssetCategoryName())
		};
	return Categories;
}

EAssetCommandResult UAssetDefinition_MyCustomAsset::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	for (const TObjectPtr<UMyCustomAsset> MyCustomAsset : OpenArgs.LoadObjects<UMyCustomAsset>())
	{
		const TSharedRef<FMyCustomAssetEditor> NewCustomAssetEditor(new FMyCustomAssetEditor());
		NewCustomAssetEditor->InitMyCustomAssetEditor(OpenArgs.GetToolkitMode(), OpenArgs.ToolkitHost, MyCustomAsset);
	}
	
	return EAssetCommandResult::Handled;
}
