// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_MyCustomAsset.h"
#include "EpicSandboxEditor.h"
#include "MyCustomAsset.h"

FColor FAssetTypeActions_MyCustomAsset::GetTypeColor() const
{
	return FColor::Emerald;
}

FText FAssetTypeActions_MyCustomAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_CustomAsset", "Custom Asset");
}

UClass* FAssetTypeActions_MyCustomAsset::GetSupportedClass() const
{
	return UMyCustomAsset::StaticClass();
}

uint32 FAssetTypeActions_MyCustomAsset::GetCategories()
{
	return FEpicSandboxEditor::GetAssetCategory();
}

// void FAssetTypeActions_MyCustomAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
// {
// 	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
// }
