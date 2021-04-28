// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"


class EPICSANDBOXEDITOR_API FAssetTypeActions_MyCustomAsset : public FAssetTypeActions_Base
{
public:	
	virtual FColor GetTypeColor() const override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
};
