﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsWorld.h"
#include "FlecsWorldSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "FlecsAbstractWorldSubsystem.generated.h"

UCLASS(Abstract)
class UNREALFLECS_API UFlecsAbstractWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);

		Collection.InitializeDependency<UFlecsWorldSubsystem>();
	}
	
	FORCEINLINE virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
	}
	
	FORCEINLINE NO_DISCARD UFlecsWorld* GetFlecsWorld() const
	{
		if (!FlecsWorld.IsValid())
		{
			FlecsWorld = UFlecsWorldSubsystem::GetDefaultWorldStatic(this);
		}

		return FlecsWorld.Get();
	}

	UPROPERTY()
	mutable TWeakObjectPtr<UFlecsWorld> FlecsWorld;
	
}; // class UFlecsAbstractWorldSubsystem
