// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "IDetailCustomization.h"

/*
 * This DetailCustomization is inspired by FWorldPartitionEditorPerProjectUserSettingsCustomization as it is one of the rare instances
 * where there is a button that the user can interact with in the DeveloperSettings ecosystem (setting config values via Editor Preferences/Project Settings).
 * Although there is an option to use a Custom Widget for UDeveloperSettings, there is seldom used.
*/

class FEpicSandboxEditorSettingsCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	
private:
	// TODO: ???
	// TArray<TSharedPtr<FString>>& GatherCSVFilesFolders() const;
	
	bool IsFileNameValid(const FString& NewName);
	
	FReply OnSettingsCSVFileCreated();
	void OnCSVNameTextChanged(const FText& InNewText);
	void OnCSVNameTextCommitted(const FText& InNewText, ETextCommit::Type InTextCommit);
	
	// Customised name edit text box used for the CSV name (without the extension or file prefix)
	// A reference is kept so that validation can be done on the input.
	TSharedPtr<SEditableTextBox> CSVFileNameTextBox;
	
	bool bIsValidNewCSVPath = true;
};
