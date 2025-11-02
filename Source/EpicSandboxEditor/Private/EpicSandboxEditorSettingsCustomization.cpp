// Copyright (c) Andrew Bell

#include "EpicSandboxEditorSettingsCustomization.h"
#include "EpicSandboxEditorSettings.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
// #include "Widgets/Input/STextComboBox.h"
#include "SPositiveActionButton.h"


TSharedRef<IDetailCustomization> FEpicSandboxEditorSettingsCustomization::MakeInstance()
{
	return MakeShared<FEpicSandboxEditorSettingsCustomization>();
}

FReply FEpicSandboxEditorSettingsCustomization::OnSettingsCSVFileCreated()
{
	static const FText Message = INVTEXT("Want to add a CSV file to process?");
	static const FText Title = INVTEXT("Continue?");
	if (FMessageDialog::Open(EAppMsgType::YesNo, Message, Title) == EAppReturnType::Yes)
	{
		GetMutableDefault<UEpicSandboxEditorSettings>()->AddSettingsCSVFile();
	}
	return FReply::Handled();
}

void FEpicSandboxEditorSettingsCustomization::OnCSVNameTextChanged(const FText& InNewText)
{
	TObjectPtr<UEpicSandboxEditorSettings> EpicSandboxEditorSettings = GetMutableDefault<UEpicSandboxEditorSettings>();
	EpicSandboxEditorSettings->CSVFileNameNoExt = InNewText.ToString();
	
	// TODO: Validate text input against existing files (DoesFileExist)...could use FPaths::ProjectContentDir() ?
	
	bIsValidNewCSVPath = IsFileNameValid(InNewText.ToString());
	
	if (!bIsValidNewCSVPath)
	{
		CSVFileNameTextBox->SetError(INVTEXT("This file already exists!"));
	}
	else
	{
		CSVFileNameTextBox->SetError(FText::GetEmpty());
	}
}

void FEpicSandboxEditorSettingsCustomization::OnCSVNameTextCommitted(const FText& InNewText, ETextCommit::Type InTextCommit)
{
	// if (bIsValidNewCSVPath && InTextCommit != ETextCommit::OnCleared)
	// {
	// 	TObjectPtr<UEpicSandboxEditorSettings> EpicSandboxEditorSettings = GetMutableDefault<UEpicSandboxEditorSettings>();
	// 	
	// 	const FScopedTransaction Transaction(INVTEXT("Set CSV Name"));W
	// 	EpicSandboxEditorSettings->Modify();
	// 	EpicSandboxEditorSettings->CSVFileNameNoExt = InNewText.ToString();
	// 	// TODO !!...still?
	// }
	// bIsValidNewCSVPath = false;
	// CSVFileNameTextBox->SetError(FText::GetEmpty());
}

bool FEpicSandboxEditorSettingsCustomization::IsFileNameValid(const FString& NewName)
{
	
	TObjectPtr<UEpicSandboxEditorSettings> EpicSandboxEditorSettings = GetMutableDefault<UEpicSandboxEditorSettings>();

	// TODO !!!! Replace TestPath with Real path
	// Also in EditorSettings class...could use the same for validation? (for now it is the below as test)
	// combine with FName::IsValidXName(NewName, INVALID_OBJECTNAME_CHARACTERS); (different invalid characters can be used) ?
	static const FString TestPath = FPaths::Combine(TEXT("Content"), TEXT("TestCSVFiles"), TEXT("/"));
	const FString NewFilePathToAdd = FPaths::Combine(TestPath, FString::Printf(TEXT("%s%s.csv"), *EpicSandboxEditorSettings->CSVFilePrefix, *NewName));
	
	for (const FFilePath& CSVFileToProcess : EpicSandboxEditorSettings->CSVFilesToProcess)
	{
		if (CSVFileToProcess.FilePath.Equals(NewFilePathToAdd))
		{
			return false;
		}
	}
	return true;
}

void FEpicSandboxEditorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	static const FString TestPath = FPaths::Combine(TEXT("Content"), TEXT("TestCSVFiles"), TEXT("/"));
	static const FName CSVFilesCategoryName("CSV Files");
	
	IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory(CSVFilesCategoryName);
	CategoryBuilder.AddCustomRow(INVTEXT("Add CSV File"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(INVTEXT("Add New CSV File"))
		.Font(DetailBuilder.GetDetailFont())
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(FText::FromString(TestPath)) ]
		// + SHorizontalBox::Slot()
		// [
		// 	// TODO !!!!
		// 	SNew(STextComboBox)
		// 	.InitiallySelectedItem(nullptr)
		// 	.OptionsSource(GatherCSVFilesFolders())
		// ]
		+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(INVTEXT("ST_")) ]
		+ SHorizontalBox::Slot()
		[
			// SAssignNew macro assigns the widget to a variable along with constructing it!
			SAssignNew(CSVFileNameTextBox, SEditableTextBox)
			.MinDesiredWidth(250.0f)
			.HintText(INVTEXT("Name of CSV file"))
			.OnTextChanged(this, &FEpicSandboxEditorSettingsCustomization::OnCSVNameTextChanged)
			.OnTextCommitted(this, &FEpicSandboxEditorSettingsCustomization::OnCSVNameTextCommitted)
		]
		+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(INVTEXT(".csv")) ]
		+ SHorizontalBox::Slot()
		.FillWidth(0.1f)
		[
			SNew(SSpacer)
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SPositiveActionButton)
			.Text(INVTEXT("Add"))
			.ToolTipText(INVTEXT("Adds a new CSV File"))
			.OnClicked(this, &FEpicSandboxEditorSettingsCustomization::OnSettingsCSVFileCreated)
			.IsEnabled_Lambda([this]() 
			{
				return bIsValidNewCSVPath;
			})
		]
	];
}
