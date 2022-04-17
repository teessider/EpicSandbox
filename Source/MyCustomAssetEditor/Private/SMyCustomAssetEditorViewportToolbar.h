// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SCommonEditorViewportToolbarBase.h"


/**
 * The level viewport toolbar widget that is placed in the viewport
 */
class SMyCustomAssetEditorViewportToolbar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(SMyCustomAssetEditorViewportToolbar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);

private:

	/**
	* Generates the Show menu content
	*/
	virtual TSharedRef<SWidget> GenerateShowMenu() const override;
};
