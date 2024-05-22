// Copyright (c) Andrew Bell


#include "MyCustomActor.h"

#include "MyCustomAsset.h"

// Sets default values
AMyCustomActor::AMyCustomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(USceneComponent::GetDefaultSceneRootVariableName());
#if WITH_EDITORONLY_DATA
	RootComponent->bVisualizeComponent = true;
#endif
	
	FirstStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("FirstStaticMeshComponent");
	FirstStaticMeshComponent->SetMobility(EComponentMobility::Movable);
	FirstStaticMeshComponent->SetupAttachment(RootComponent);

	SecondStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SecondStaticMeshComponent");
	SecondStaticMeshComponent->SetMobility(EComponentMobility::Movable);
	SecondStaticMeshComponent->SetupAttachment(RootComponent);
}

void AMyCustomActor::SetMyCustomAsset(const TObjectPtr<UMyCustomAsset>& InMyCustomAsset)
{
	MyCustomAsset = *InMyCustomAsset;
}

void AMyCustomActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName;
	if (PropertyChangedEvent.Property)
	{
		PropertyName = PropertyChangedEvent.Property->GetFName();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMyCustomActor, MyCustomAsset))
	{
		/* TODO:
		 *  Change/set up the static mesh components again based on what is set in MyCustomAsset
		 */
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
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

