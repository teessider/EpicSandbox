// Copyright (c) Andrew Bell


#include "MyCustomAsset.h"
#include "UObject/AssetRegistryTagsContext.h"

UMyCustomAsset::UMyCustomAsset()
{
	
}

void UMyCustomAsset::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{

	Super::GetAssetRegistryTags(Context);
	
#if WITH_EDITORONLY_DATA
	Context.AddTag(FAssetRegistryTag("FirstCustomMesh", FirstCustomStaticMesh ? TEXT("UObject Name ") + FirstCustomStaticMesh->GetName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	Context.AddTag(FAssetRegistryTag("SecondCustomMesh", SecondCustomStaticMesh ? TEXT("UObject FName (Logical Name) ") + SecondCustomStaticMesh->GetFName().ToString() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
	// Context.AddTag(FAssetRegistryTag("CustomInt", FString::FromInt(CustomInt), FAssetRegistryTag::TT_Numerical));  // Alternative to AssetRegistrySearchable UPROPERTY metadata
	Context.AddTag(FAssetRegistryTag("IsActive", bIsActive ? TEXT("True") : TEXT("False"), FAssetRegistryTag::TT_Alphabetical));
	Context.AddTag(FAssetRegistryTag("Description", Description.IsEmpty() ? TEXT("Empty") : Description, FAssetRegistryTag::TT_Alphabetical));
	Context.AddTag(FAssetRegistryTag("CustomDataAsset", CustomDataAsset ? CustomDataAsset->GetName() : TEXT("None"), FAssetRegistryTag::TT_Alphabetical));
#endif
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

	OutMetadata.Add("CustomDataAsset",
		FAssetRegistryTagMetadata()
		.SetImportantValue(TEXT("None"))
		);
}

EDataValidationResult UMyCustomAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	// If returning ONLY the EDataValidationResult enum entries, the "normal" Data Validation system is not used so one has to add their own texts there...
	// (makes sense as AssetPasses/AssetFails methods come from the the Data Validation Editor Subsystem... )
	// TODO: Make a basic copy of UEditorValidatorBase::AssetPasses and UEditorValidatorBase::AssetFails
	if(Description.IsEmpty()) 
	{
		return EDataValidationResult::Invalid;
	}
	return EDataValidationResult::Valid;
}
#endif
