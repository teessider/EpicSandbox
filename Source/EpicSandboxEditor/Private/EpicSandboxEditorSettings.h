// Copyright (c) Andrew Bell

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "EpicSandboxEditorSettings.generated.h"

UCLASS(config = Editor, DefaultConfig, MinimalAPI, meta=(DisplayName="Epic Sandbox (Editor Only)"))
class UEpicSandboxEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UEpicSandboxEditorSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	FString CSVFileNameNoExt;

	UPROPERTY()
	FString CSVFilePrefix;

	UPROPERTY()
	TArray<FString> CSVFileFolders;
	
	UPROPERTY(config, EditAnywhere, Category="CSV Files", meta=(RelativeToGameDir, FilePathFilter="CSV (*.csv)|*.csv"))
	TArray<FFilePath> CSVFilesToProcess;

	void AddSettingsCSVFile();
};
