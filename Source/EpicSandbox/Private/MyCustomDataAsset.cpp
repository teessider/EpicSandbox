// Copyright (c) Andrew Bell


#include "MyCustomDataAsset.h"

#include "UObject/AssetRegistryTagsContext.h"

UMyCustomDataAsset::UMyCustomDataAsset()
{
}

void UMyCustomDataAsset::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);
	
	Context.AddTag(FAssetRegistryTag("DataDescription", DataDescription.IsEmpty() ? TEXT("Empty") : DataDescription, FAssetRegistryTag::TT_Alphabetical));
	Context.AddTag(FAssetRegistryTag("IsDataActive", bIsDataActive ? TEXT("True") : TEXT("False"), FAssetRegistryTag::TT_Alphabetical));

}

void UMyCustomDataAsset::GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const
{
	Super::GetAssetRegistryTagMetadata(OutMetadata);

	OutMetadata.Add("DataDescription",
		FAssetRegistryTagMetadata()
		.SetImportantValue(TEXT("Empty"))
		);
}
