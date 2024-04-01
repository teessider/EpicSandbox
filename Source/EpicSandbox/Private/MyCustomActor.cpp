// Copyright (c) Andrew Bell


#include "MyCustomActor.h"

// Sets default values
AMyCustomActor::AMyCustomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCustomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

