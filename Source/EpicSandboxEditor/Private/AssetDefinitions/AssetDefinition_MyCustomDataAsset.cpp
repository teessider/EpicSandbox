// Copyright (c) Andrew Bell


#include "AssetDefinition_MyCustomDataAsset.h"
#include "MyCustomDataAsset.h"
#include "EpicSandboxEditor.h"

FText UAssetDefinition_MyCustomDataAsset::GetAssetDisplayName() const
{
	return INVTEXT("Custom Data Asset");
}

FLinearColor UAssetDefinition_MyCustomDataAsset::GetAssetColor() const
{
	return FLinearColor::Blue;
}

TSoftClassPtr<> UAssetDefinition_MyCustomDataAsset::GetAssetClass() const
{
	return UMyCustomDataAsset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_MyCustomDataAsset::GetAssetCategories() const
{
	static const auto Categories =
		{
			FAssetCategoryPath(FEpicSandboxEditor::GetAssetCategoryName())
		};
	return Categories;
}
