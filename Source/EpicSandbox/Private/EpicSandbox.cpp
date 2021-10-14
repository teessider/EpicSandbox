// Fill out your copyright notice in the Description page of Project Settings.

#include "EpicSandbox.h"
#include "Modules/ModuleManager.h"

#include "Misc/Paths.h"
#include "ShaderCore.h"

void FEpicSandbox::StartupModule()
{
	const FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));

	AddShaderSourceDirectoryMapping(TEXT("/Project"), ShaderDirectory);
}

void FEpicSandbox::ShutdownModule()
{
	ResetAllShaderSourceDirectoryMappings();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, EpicSandbox, "EpicSandbox" );
