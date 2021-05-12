// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomAsset.h"

UMyCustomAsset::UMyCustomAsset()
{
	
}

void UMyCustomAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	
#if WITH_EDITORONLY_DATA
	OutTags.Add(FAssetRegistryTag("FirstCustomMesh", FirstCustomStaticMesh ? TEXT("UObject Name ") + FirstCustomStaticMesh->GetName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("SecondCustomMesh", SecondCustomStaticMesh ? TEXT("UObject FName (Logical Name) ") + SecondCustomStaticMesh->GetFName().ToString() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("CustomStructMesh", CustomStructMesh.CustomStructMesh ? TEXT("UObject Path Name ") + CustomStructMesh.CustomStructMesh->GetPathName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("CustomStructMeshName", CustomStructMesh.MeshName.IsValid() ? CustomStructMesh.MeshName.ToString() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	// OutTags.Add(FAssetRegistryTag("CustomInt", FString::FromInt(CustomInt), FAssetRegistryTag::TT_Numerical));  // Alternative to AssetRegistrySearchable UPROPERTY metadata
	OutTags.Add(FAssetRegistryTag("IsActive", bIsActive ? TEXT("True") : TEXT("False"), FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("Description", Description.IsEmpty() ? TEXT("Empty") : Description, FAssetRegistryTag::TT_Alphabetical));
	OutTags.Add(FAssetRegistryTag("CustomDataAsset", CustomDataAsset ? CustomDataAsset->GetName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
#endif

	Super::GetAssetRegistryTags(OutTags);
}

#if WITH_EDITOR
void UMyCustomAsset::GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const
{
	Super::GetAssetRegistryTagMetadata(OutMetadata);

	// The InKey should be the same as the Asset Registry Tag "InName" above!
	OutMetadata.Add("IsActive",
		FAssetRegistryTagMetadata()
		.SetDisplayName(FText::FromString("Is Active")) //I know this is not good ;)
		.SetSuffix(FText::FromString("|The Suffix!"))
		.SetTooltip(NSLOCTEXT("UMyCustomAsset", "IsActiveTooltip", "Is this Asset Active ?"))
		.SetImportantValue(TEXT("True"))
		);
}
#endif
