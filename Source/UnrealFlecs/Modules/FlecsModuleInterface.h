﻿// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable IdentifierTypo
#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "Entities/FlecsEntityHandle.h"
#include "SolidMacros/Macros.h"
#include "UObject/Interface.h"
#include "FlecsModuleInterface.generated.h"

class UFlecsWorld;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UNREALFLECS_API UFlecsModuleInterface : public UInterface
{
	GENERATED_BODY()
}; // class UFlecsModuleInterface

class UNREALFLECS_API IFlecsModuleInterface
{
	GENERATED_BODY()

public:

	FORCEINLINE void ImportModule(flecs::world& InWorld);
	
	FORCEINLINE void DeinitializeModule_Internal();

	virtual void InitializeModule(UFlecsWorld* InWorld, const FFlecsEntityHandle& InModuleEntity);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Flecs | Modules", meta = (DisplayName = "Initialize Module"))
	void BP_InitializeModule(UFlecsWorld* InWorld);

	virtual void DeinitializeModule(UFlecsWorld* InWorld);

	UFUNCTION(BlueprintNativeEvent, Category = "Flecs | Modules", meta = (DisplayName = "Deinitialize Module"))
	void BP_DeinitializeModule(UFlecsWorld* InWorld);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Flecs | Modules")
	FString GetModuleName() const;
	virtual FString GetModuleName_Implementation() const;

	FORCEINLINE NO_DISCARD UFlecsWorld* GetFlecsWorld() const
	{
		return World.Get();
	}

	FORCEINLINE NO_DISCARD bool IsWorldValid() const
	{
		return World.IsValid();
	}

	FORCEINLINE NO_DISCARD FFlecsEntityHandle GetModuleEntity() const
	{
		return ModuleEntity;
	}
	
	TWeakObjectPtr<UFlecsWorld> World;

	FFlecsEntityHandle ModuleEntity;
	
}; // class IFlecsModuleInterface
