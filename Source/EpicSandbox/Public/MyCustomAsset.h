// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyCustomDataAsset.h"
#include "UObject/Object.h"
#include "MyCustomAsset.generated.h"

/**
 * 
 */
UCLASS()
class EPICSANDBOX_API UMyCustomAsset : public UObject
{
	GENERATED_BODY()

public:
	UMyCustomAsset();
	
	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(UIMin=0, UIMax=10), AssetRegistrySearchable)
	int32 CustomInt;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset)
	UMyCustomDataAsset* CustomDataAsset;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(EditCondition="CustomInt > 5"))
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset)
	class UStaticMesh* CustomStaticMesh;

	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="MyCustomAsset|Description", meta=(InlineEditCondition))
	bool bEnableDescription;
	
	UPROPERTY(EditDefaultsOnly, Category="MyCustomAsset|Description", meta=(EditCondition="bEnableDescription"))
	FString Description;
};
