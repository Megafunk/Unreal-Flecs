﻿// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppExpressionWithoutSideEffects
#include "FlecsRestModule.h"
#include "Worlds/FlecsWorld.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlecsRestModule)

void UFlecsRestModule::InitializeModule(UFlecsWorld* InWorld, const FFlecsEntityHandle& InModuleEntity)
{
	int32 ClientPieInstanceOffset = 0;
	
	const UWorld* UnrealWorld = InWorld->GetWorld();
	if (UnrealWorld && UnrealWorld->GetNetMode() == NM_Client)
	{
		ClientPieInstanceOffset = UE::GetPlayInEditorID();
	}
	
	// ECS_REST_DEFAULT_PORT is 27750
	uint16 RestPort = 27750 + ClientPieInstanceOffset;

	InWorld->SetSingleton<flecs::Rest>(flecs::Rest{ .port = RestPort});
	RestEntity = InWorld->GetSingletonEntity<flecs::Rest>();

	if (bImportStats)
	{
		StatsEntity = InWorld->World.import<flecs::stats>();
	}
}

void UFlecsRestModule::DeinitializeModule(UFlecsWorld* InWorld)
{
	if (StatsEntity.IsValid())
	{
		StatsEntity.Disable();
	}
	
	if UNLIKELY_IF(!RestEntity.IsValid())
	{
		return;
	}
	
	RestEntity.Disable();
}
