// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomDataAssetFactory.h"
#include "MyCustomDataAsset.h"

UMyCustomDataAssetFactory::UMyCustomDataAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	// bEditorImport = false;  // For now, this doesn't import anything...
	SupportedClass = UMyCustomDataAsset::StaticClass();
}

UObject* UMyCustomDataAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMyCustomDataAsset* MyCustomDataAsset = NewObject<UMyCustomDataAsset>(InParent, InClass, InName, Flags);
	return MyCustomDataAsset;
}
