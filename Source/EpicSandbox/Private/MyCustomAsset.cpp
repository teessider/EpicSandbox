// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomAsset.h"

UMyCustomAsset::UMyCustomAsset()
{
	
}

void UMyCustomAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	OutTags.Add(FAssetRegistryTag("Mesh", CustomStaticMesh ? CustomStaticMesh->GetName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	// OutTags.Add(FAssetRegistryTag("CustomInt", FString::FromInt(CustomInt), FAssetRegistryTag::TT_Numerical));
	OutTags.Add(FAssetRegistryTag("IsActive", bIsActive ? TEXT("True") : TEXT("False"), FAssetRegistryTag::TT_Alphabetical));
	Super::GetAssetRegistryTags(OutTags);
}