// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCustomActor.generated.h"

UCLASS(HideCategories=("Replication", "Networking", "HLOD", "Input", "Rendering", "Collision", "Physics", "LevelInstance", "Cooking", "Actor", "DataLayers"))
class EPICSANDBOX_API AMyCustomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCustomActor();

	TObjectPtr<class UMyCustomAsset> GetMyCustomAsset() const { return MyCustomAsset; }
	void SetMyCustomAsset(const TObjectPtr<UMyCustomAsset>& InMyCustomAsset);

	TObjectPtr<UStaticMeshComponent> GetFirstStaticMeshComponent() const { return FirstStaticMeshComponent; }
	TObjectPtr<UStaticMeshComponent> GetSecondStaticMeshComponent() const { return SecondStaticMeshComponent; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta=(DisplayPriority=0))
	TObjectPtr<UMyCustomAsset> MyCustomAsset;

	UPROPERTY(VisibleDefaultsOnly, Category=MyCustomActor)
	TObjectPtr<UStaticMeshComponent> FirstStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category=MyCustomActor)
	TObjectPtr<UStaticMeshComponent> SecondStaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
