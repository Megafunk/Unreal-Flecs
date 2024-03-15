﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"
#include "Runtime/SlateCore/Public/Brushes/SlateImageBrush.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyleRegistry.h"

class FUnrealFlecsEditorStyle : public FEditorStyle
{
public:
	FORCEINLINE static void Initialize()
	{
		FString PluginContentDir = IPluginManager::Get().FindPlugin(TEXT("UnrealFlecs"))->GetBaseDir();
		const FString ContentDir = FPaths::Combine(PluginContentDir, TEXT("Resources"));

		FSlateStyleSet* StyleSet = new FSlateStyleSet("UnrealFlecsEditorStyle");
		StyleSet->SetContentRoot(ContentDir);

		const FVector2D IconSize(128.0f, 128.0f);
		const FString IconPath = FPaths::Combine(ContentDir, TEXT("Icon128.png"));
		StyleSet->Set("UnrealFlecs.FlecsEditor.FlecsLogo",
			new FSlateImageBrush(IconPath, IconSize));

		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}

	FORCEINLINE static void Shutdown()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle("UnrealFlecsEditorStyle");
	}
	
}; // struct FUnrealFlecsEditorStyle
