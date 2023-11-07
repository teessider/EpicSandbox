// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"

class UMyCustomAsset;

class IDetailsView;
class SDockTab;

class MYCUSTOMASSETEDITOR_API FMyCustomAssetEditor final : public FAssetEditorToolkit, public FGCObject
{
public:
	FMyCustomAssetEditor()
		: MyCustomAssetBeingEdited(nullptr)
	{}
	
	/** Destructor */
	virtual ~FMyCustomAssetEditor() override;

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	// End of IToolkit interface
	
	void InitMyCustomAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, class UMyCustomAsset* InMyCustomAsset);
	
	
	//~ Begin FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End FAssetEditorToolkit

	// FSerializableObject interface
	// This comes from FGCObject class
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	// End of FSerializableObject interface

private:
	/** The currently viewed MyCustomAsset which potentially contains many meshes */
	UMyCustomAsset* MyCustomAssetBeingEdited;

	
	
	/** Create the different tabs and their content */
	TSharedRef<SDockTab> SpawnTab_Details( const FSpawnTabArgs& Args ) const;
	TSharedRef<SDockTab> SpawnTab_Viewport( const FSpawnTabArgs& Args ) const;

	/** Details view */
	TSharedPtr<class IDetailsView> DetailsView;
	// /** Preview Viewport (Preview Scene) Widget */
	TSharedPtr<class SMyCustomAssetEditorViewport> Viewport;
	
	/** Custom Asset Editor App Identifier String */
	static const FName CustomAssetEditorAppIdentifier;

	/**	The tab ids for all the tabs used */
	static const FName DetailsTabId;
	static const FName ViewportTabId;
};