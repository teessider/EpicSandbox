// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "ActorFactories/ActorFactory.h"

#include "MyCustomAssetActorFactory.generated.h"

UCLASS()
class EPICSANDBOXEDITOR_API UMyCustomAssetActorFactory : public UActorFactory
{
	GENERATED_BODY()
	
public:
	UMyCustomAssetActorFactory(const FObjectInitializer& ObjectInitializer);

	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	virtual UObject* GetAssetFromActorInstance(AActor* ActorInstance) override;

protected:
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
};
