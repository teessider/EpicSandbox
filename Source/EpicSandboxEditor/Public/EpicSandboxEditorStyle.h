// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class EPICSANDBOXEDITOR_API FEpicSandboxEditorStyle
{
public:
	static void Initialize();

	static void Shutdown();

	static FName GetStyleSetName();

private:
	static TSharedPtr<class FSlateStyleSet> EditorStyle;
};
