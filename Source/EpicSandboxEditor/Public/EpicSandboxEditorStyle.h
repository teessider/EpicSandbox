// Copyright (c) Andrew Bell

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
