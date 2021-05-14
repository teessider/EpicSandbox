// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "AssetTypeCategories.h"
#include "Styling/SlateStyle.h"

/* Following the NiagaraEditor way of doing AssetTypeActions allows for an easier way of doing multiple AssetTypeActions!
 * - Add private static EAssetTypeCategories::Type with a Getter (getter is for the AssetTypeActions GetCategories() method)
 * - - Forward Declare that in Editor cpp
 * - RegisterAssetTypeAction() which does the usual RegisterAssetTypeActions()
 * - - but also adds to a cached version of AssetTypeActions (CreatedAssetTypeActions) which allows them to be unregistered
 *
 */

// Forward declaration - only need these classes
class IAssetTools;
class IAssetTypeActions;

DECLARE_LOG_CATEGORY_EXTERN(LogEpicSandboxEditor, All, All)

class EPICSANDBOXEDITOR_API FEpicSandboxEditor : public IModuleInterface
{
public:
	// Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface implementation

	TSharedPtr<FSlateStyleSet> EpicSandboxEditorStyle;  // Could be in own module or plugin!

	static EAssetTypeCategories::Type GetAssetCategory() { return EpicSandboxAssetCategory; }

private:
	static EAssetTypeCategories::Type EpicSandboxAssetCategory;

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

	/** All created asset type actions.  Cached here so that they can be unregistered during shutdown. */
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;
};
