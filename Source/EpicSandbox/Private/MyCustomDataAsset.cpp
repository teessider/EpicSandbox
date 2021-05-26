// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomDataAsset.h"

UMyCustomDataAsset::UMyCustomDataAsset()
{
}

void UMyCustomDataAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	Super::GetAssetRegistryTags(OutTags);
	
	OutTags.Add(FAssetRegistryTag("DataDescription", DataDescription.IsEmpty() ? TEXT("Empty") : DataDescription, FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("IsDataActive", bIsDataActive ? TEXT("True") : TEXT("False"), FAssetRegistryTag::TT_Alphabetical));

}
