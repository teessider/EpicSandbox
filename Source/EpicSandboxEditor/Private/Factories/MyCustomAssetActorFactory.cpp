// Copyright (c) Andrew Bell


#include "MyCustomAssetActorFactory.h"

#include "MyCustomActor.h"
#include "MyCustomAsset.h"

UMyCustomAssetActorFactory::UMyCustomAssetActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = INVTEXT("My Custom Asset");
	NewActorClass = AMyCustomActor::StaticClass();
	bUseSurfaceOrientation = true;
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

UObject* UMyCustomAssetActorFactory::GetAssetFromActorInstance(AActor* ActorInstance)
{
	check(ActorInstance->IsA(NewActorClass));
	const AMyCustomActor* MyCustomActor = CastChecked<AMyCustomActor>(ActorInstance);
	
	return MyCustomActor->GetMyCustomAsset();
}

void UMyCustomAssetActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	const TObjectPtr<UMyCustomAsset> MyCustomAsset = Cast<UMyCustomAsset>(Asset);
	TObjectPtr<AMyCustomActor> MyCustomActor = CastChecked<AMyCustomActor>(NewActor);

	if(Asset == nullptr)
	{
		return;
	}
	
	MyCustomActor->SetMyCustomAsset(MyCustomAsset);

	// Every component which is visual (Static Mesh, Skeletal Mesh, Decal etc.) in ActorFactory.cpp,
	// unregisters the component, does something to change properties (like set a material or a static mesh), then registers the component.
	// For both Static Mesh Components, I will do the same
	if (const TObjectPtr<UStaticMeshComponent> FirstStaticMeshComponent = MyCustomActor->GetFirstStaticMeshComponent())
	{
		FirstStaticMeshComponent->UnregisterComponent();
		
		const TObjectPtr<UStaticMesh> FirstStaticMesh = MyCustomAsset->GetFirstStaticMesh();
		FirstStaticMeshComponent->SetStaticMesh(FirstStaticMesh);
		if (FirstStaticMesh->GetRenderData())
		{
			FirstStaticMeshComponent->StaticMeshDerivedDataKey = FirstStaticMesh->GetRenderData()->DerivedDataKey;
		}
		
		FirstStaticMeshComponent->RegisterComponent();
	}
	
	if (const TObjectPtr<UStaticMeshComponent> SecondStaticMeshComponent = MyCustomActor->GetSecondStaticMeshComponent())
	{
		SecondStaticMeshComponent->UnregisterComponent();
		
		const TObjectPtr<UStaticMesh> SecondStaticMesh = MyCustomAsset->GetSecondStaticMesh();
		SecondStaticMeshComponent->SetStaticMesh(SecondStaticMesh);
		if (SecondStaticMesh->GetRenderData())
		{
			SecondStaticMeshComponent->StaticMeshDerivedDataKey = SecondStaticMesh->GetRenderData()->DerivedDataKey;
		}
		
		SecondStaticMeshComponent->RegisterComponent();
	}
}
