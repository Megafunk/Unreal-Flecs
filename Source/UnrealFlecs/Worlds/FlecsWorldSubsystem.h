﻿// Solstice Games © 2024. All Rights Reserved.

#pragma once

#include <unordered_map>

#include "CoreMinimal.h"
#include "flecs.h"
#include "FlecsWorld.h"
#include "FlecsWorldSettings.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "SolidMacros/Standard/Hashing.h"
#include "Subsystems/WorldSubsystem.h"
#include "FlecsWorldSubsystem.generated.h"

const FName DEFAULT_FLECS_WORLD_NAME = "DefaultFlecsWorld";

UCLASS(BlueprintType)
class UNREALFLECS_API UFlecsWorldSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);

		CreateWorld(DEFAULT_FLECS_WORLD_NAME, FFlecsWorldSettings());
	}

	void Deinitialize() override
	{
		Super::Deinitialize();

		DestroyWorldByName(DEFAULT_FLECS_WORLD_NAME);
	}
	
	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FFlecsWorld& CreateWorld(const FName& Name, const FFlecsWorldSettings& Settings)
	{
		flecs::world NewWorld = flecs::world();
		FFlecsWorld World(NewWorld);
		
		Worlds.emplace_back(World);
		WorldNameMap[Name] = &Worlds.back();

		World->set<FName>(Name);
		World->set_ctx(this);

		World->set_automerge(Settings.bAutoMerge);
		
		return Worlds.back();
	}

	template <typename T>
	void ImportModule(const FName& WorldName) const
	{
		GetFlecsWorld(WorldName)->import<T>();
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs", BlueprintPure = false)
	void SetAutoMerge(const FName& Name, const bool bAutoMerge) const
	{
		GetFlecsWorld(Name)->set_automerge(bAutoMerge);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FFlecsWorld& GetFlecsWorld(const FName& Name) const
	{
		return *WorldNameMap.at(Name);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs", Meta = (WorldContext = "WorldContextObject"))
	static FFlecsWorld& GetWorldStatic(UObject* WorldContextObject, const FName& Name)
	{
		return GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)
		              ->GetSubsystem<UFlecsWorldSubsystem>()->GetFlecsWorld(Name);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs", Meta = (WorldContext = "WorldContextObject"))
	static FFlecsWorld& GetDefaultWorld(const UObject* WorldContextObject)
	{
		return GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)
		              ->GetSubsystem<UFlecsWorldSubsystem>()->GetFlecsWorld(DEFAULT_FLECS_WORLD_NAME);
	}
	
	UFUNCTION(BlueprintCallable, Category = "Flecs")
	void DestroyWorldByName(const FName& Name)
	{
		WorldNameMap.at(Name)->GetWorld().init_builtin_components();
		WorldNameMap.erase(Name);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	void DestroyWorld(FFlecsWorld& World)
	{
		const FFlecsWorld* WorldPtr = WorldNameMap.at(*World->get<FName>());
		WorldNameMap.erase(*World->get<FName>());

		for (auto It = Worlds.begin(); It != Worlds.end(); ++It)
		{
			if (&*It == WorldPtr)
			{
				Worlds.erase(It);
				break;
			}
		}
	}
	
	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
	}

protected:
	std::vector<FFlecsWorld> Worlds;
	std::unordered_map<FName, FFlecsWorld*> WorldNameMap;
}; // class UFlecsWorldSubsystem
