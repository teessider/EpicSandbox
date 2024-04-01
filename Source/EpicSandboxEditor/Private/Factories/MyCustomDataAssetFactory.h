// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "MyCustomDataAssetFactory.generated.h"

UCLASS()
class EPICSANDBOXEDITOR_API UMyCustomDataAssetFactory final : public UFactory
{
	GENERATED_BODY()
	
public:
	UMyCustomDataAssetFactory();

	// Begin UFactory Implementation
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End UFactory Implementation
};
