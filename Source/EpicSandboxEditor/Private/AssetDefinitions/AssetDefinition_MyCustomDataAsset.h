// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"


#include "AssetDefinition_MyCustomDataAsset.generated.h"

UCLASS()
class EPICSANDBOXEDITOR_API UAssetDefinition_MyCustomDataAsset : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};
