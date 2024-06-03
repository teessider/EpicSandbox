// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyCustomDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EPICSANDBOX_API UMyCustomDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMyCustomDataAsset();
	
	UPROPERTY(EditAnywhere, Category=MyCustomDataAsset, meta=(UIMin=-10, UIMax=10), AssetRegistrySearchable)
	int32 CustomDataInt;

	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

#if WITH_EDITOR
	virtual void GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const override;
#endif
	
protected:
	UPROPERTY(EditAnywhere, Category=MyCustomDataAsset, meta=(EditCondition="bIsDataActive", DisplayAfter="bIsDataActive"))
	FString DataDescription;

	UPROPERTY(EditAnywhere, Category=MyCustomDataAsset, meta=(InlineEditCondition, DisplayName="Is Data Active?"))
	bool bIsDataActive;
};
