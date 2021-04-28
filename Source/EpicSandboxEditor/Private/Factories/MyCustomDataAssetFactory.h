// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "MyCustomDataAssetFactory.generated.h"

UCLASS()
class EPICSANDBOXEDITOR_API UMyCustomDataAssetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UMyCustomDataAssetFactory();

	// Begin UFactory Implementation
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End UFactory Implementation
};
