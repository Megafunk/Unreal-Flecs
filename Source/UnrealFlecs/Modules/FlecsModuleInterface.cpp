﻿// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppExpressionWithoutSideEffects
#include "FlecsModuleInterface.h"
#include "FlecsModuleInitEvent.h"
#include "Components/FlecsModuleComponent.h"
#include "Components/FlecsWorldPtrComponent.h"
#include "Logs/FlecsCategories.h"
#include "Unlog/Unlog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlecsModuleInterface)

void IFlecsModuleInterface::ImportModule(flecs::world& InWorld)
{
	World = ToFlecsWorld(InWorld);
	solid_check(World.IsValid());

	const FFlecsEntityHandle OldScope = World->ClearScope();

	ModuleEntity = World->CreateEntity(Execute_GetModuleName(_getUObject()));
	solid_check(ModuleEntity.IsValid());
	
	ModuleEntity.Add(flecs::Module);
	ModuleEntity.Set<FFlecsUObjectComponent>({ _getUObject() });
	ModuleEntity.Set<FFlecsModuleComponent>({ _getUObject()->GetClass() });

	World->SetScope(ModuleEntity);
		
	InitializeModule(World.Get(), ModuleEntity);
	Execute_BP_InitializeModule(_getUObject(), World.Get());
	
	World->SetScope(OldScope);

	World->Event<FFlecsModuleInitEvent>()
		.id<FFlecsModuleComponent>()
		.entity(ModuleEntity)
		.emit();
	
	UN_LOGF(LogFlecsCore, Log,
		"Imported module: %s", *IFlecsModuleInterface::Execute_GetModuleName(_getUObject()));
}

inline void IFlecsModuleInterface::DeinitializeModule_Internal()
{
	ModuleEntity.Disable();
	DeinitializeModule(World.Get());
	Execute_BP_DeinitializeModule(_getUObject(), World.Get());

	UN_LOGF(LogFlecsCore, Log, "Deinitialized module: %s",
		*IFlecsModuleInterface::Execute_GetModuleName(_getUObject()));
}

void IFlecsModuleInterface::InitializeModule(UFlecsWorld* InWorld, const FFlecsEntityHandle& InModuleEntity)
{
}

void IFlecsModuleInterface::DeinitializeModule(UFlecsWorld* InWorld)
{
}

FString IFlecsModuleInterface::GetModuleName_Implementation() const
{
	UN_LOGF(LogFlecsCore, Warning,
		"Module name not implemented! It is recommended to implement this function in the module interface. "
		"Will return the inherited class name instead. For Class: %s", *_getUObject()->GetClass()->GetName());
	
	return _getUObject()->GetClass()->GetName();
}
