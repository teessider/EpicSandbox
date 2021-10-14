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
// Commented out as this is actually NOT needed if SupportedClass is the corresponding Abstract/Base Class (the final one)
// Kept though for reference
// FName UMyCustomAssetFactory::GetNewAssetThumbnailOverride() const
// {
// 	// This is set in the Slate Style (currently in the EpicSandboxEditorStyle module) and needs to be the same string...
// 	return TEXT("ClassThumbnail.MyCustomAsset");
// }

// bool UMyCustomAssetFactory::ConfigureProperties()
// {
// 	//TODO: Add a example dialog picker here...
// 	
// 	//	Check if UMyCustomAsset is has UFactory equiv ?	
// 	
// }

UObject* UMyCustomAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMyCustomAsset* MyCustomAsset = NewObject<UMyCustomAsset>(InParent, InClass, InName, Flags);
	return MyCustomAsset;
}
