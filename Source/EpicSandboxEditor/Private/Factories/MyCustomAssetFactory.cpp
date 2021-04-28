// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomAssetFactory.h"
#include "MyCustomAsset.h"

UMyCustomAssetFactory::UMyCustomAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	// bEditorImport = false;  // For now, this doesn't import anything...
	SupportedClass = UMyCustomAsset::StaticClass();
}

UObject* UMyCustomAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMyCustomAsset* MyCustomAsset = NewObject<UMyCustomAsset>(InParent, InClass, InName, Flags);
	return MyCustomAsset;
}
