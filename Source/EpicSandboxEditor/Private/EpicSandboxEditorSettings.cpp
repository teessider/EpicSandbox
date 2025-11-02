// Copyright (c) Andrew Bell


#include "EpicSandboxEditorSettings.h"


UEpicSandboxEditorSettings::UEpicSandboxEditorSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CSVFileNameNoExt = TEXT("");
	CSVFilePrefix = TEXT("ST_");
}

void UEpicSandboxEditorSettings::AddSettingsCSVFile()
{
	// TODO: Replace TestPath with real path...
	// TODO: make file extension a variable?
	static const FString TestPath = FPaths::Combine(TEXT("Content"), TEXT("TestCSVFiles"));

	const FString FilePathToAdd = FPaths::Combine(TestPath, FString::Printf(TEXT("%s%s.csv"), *CSVFilePrefix, *CSVFileNameNoExt));
	
	CSVFilesToProcess.Add(FFilePath(FilePathToAdd));
	TryUpdateDefaultConfigFile();
}
