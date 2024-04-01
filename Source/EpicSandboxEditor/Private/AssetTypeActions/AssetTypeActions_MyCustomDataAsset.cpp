// Copyright (c) Andrew Bell


#include "AssetTypeActions_MyCustomDataAsset.h"
#include "EpicSandboxEditor.h"
#include "MyCustomDataAsset.h"

FColor FAssetTypeActions_MyCustomDataAsset::GetTypeColor() const
{
	return FColor::Blue;
}

FText FAssetTypeActions_MyCustomDataAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_CustomDataAsset", "Custom Data Asset");
}

UClass* FAssetTypeActions_MyCustomDataAsset::GetSupportedClass() const
{
	return UMyCustomDataAsset::StaticClass();
}

uint32 FAssetTypeActions_MyCustomDataAsset::GetCategories()
{
	return FEpicSandboxEditor::GetAssetCategory();
}
