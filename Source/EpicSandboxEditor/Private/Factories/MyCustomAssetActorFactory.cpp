﻿// Copyright (c) Andrew Bell


#include "MyCustomAssetActorFactory.h"

#include "MyCustomActor.h"
#include "MyCustomAsset.h"

UMyCustomAssetActorFactory::UMyCustomAssetActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = INVTEXT("My Custom Asset");
	NewActorClass = AMyCustomActor::StaticClass();
}

bool UMyCustomAssetActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if ( !AssetData.IsValid() || !AssetData.IsInstanceOf( UMyCustomAsset::StaticClass() ) )
	{
		OutErrorMsg = INVTEXT("A valid My Custom Asset must be specified.");
		return false;
	}

	return true;
}