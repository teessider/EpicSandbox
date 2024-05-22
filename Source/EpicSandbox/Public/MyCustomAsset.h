// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"

#include "MyCustomDataAsset.h"
#include "UObject/Object.h"
#include "MyCustomAsset.generated.h"

USTRUCT()
struct FCustomMeshStruct
{
	GENERATED_BODY()

	// Adding this makes the MeshName property marked as changed... Wonder if there is a way to make not do that...
	FCustomMeshStruct()
		: MeshName("CustomMeshName")
	{
		CustomStructMesh = nullptr;
	}
	
	UPROPERTY(EditDefaultsOnly)
	FName MeshName;

	UPROPERTY(EditDefaultsOnly, meta=(DisplayName = "Struct Static Mesh"))
	TObjectPtr<UStaticMesh> CustomStructMesh;
};

/**
 * 
 */
UCLASS(BlueprintType, meta=(LoadBehavior="LazyOnDemand"))
class EPICSANDBOX_API UMyCustomAsset : public UObject
{
	GENERATED_BODY()

public:
	UMyCustomAsset();
	
	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(UIMin = 0, UIMax = 10, DisplayAfter = "CustomDataAsset"), AssetRegistrySearchable)
	int32 CustomInt;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(DisplayAfter = "bIsActive"))
	UMyCustomDataAsset* CustomDataAsset;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(EditCondition = "CustomInt > 5"))
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(DisplayName = "First Static Mesh"))
	TObjectPtr<class UStaticMesh> FirstCustomStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(DisplayName = "Second Static Mesh"))
	TObjectPtr<class UStaticMesh> SecondCustomStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(DisplayName = "Struct Mesh"))
	FCustomMeshStruct CustomStructMesh;

	UPROPERTY(EditDefaultsOnly, Category=MyCustomAsset, meta=(DisplayName = "List of Struct Static Meshes", TitleProperty = "MeshName"))
	TArray<FCustomMeshStruct> CustomStaticMeshes;

	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;
	
#if WITH_EDITOR
	virtual void GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const override;

	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

protected:
	UPROPERTY(EditDefaultsOnly, Category="MyCustomAsset|Description", meta=(InlineEditConditionToggle)) //InlineEditConditionTOGGLE inlines the bool visually (puts it on the same row)
	bool bEnableDescription;
	
	UPROPERTY(EditDefaultsOnly, Category="MyCustomAsset|Description", meta=(EditCondition = "bEnableDescription"))
	FString Description;
};
