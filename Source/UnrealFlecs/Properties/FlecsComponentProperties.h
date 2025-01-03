﻿// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppExpressionWithoutSideEffects
#pragma once

#include <string>
#include <vector>

#include "CoreMinimal.h"
#include "flecs.h"
#include "Standard/robin_hood.h"
#include "SolidMacros/Macros.h"
#include "StructUtils/InstancedStruct.h"
#include "Unlog/Unlog.h"

struct UNREALFLECS_API FFlecsComponentProperties
{
	std::string Name;
	std::vector<flecs::entity_t> Entities;
	
	TArray<FInstancedStruct> ComponentPropertyStructs;
}; // struct FFlecsComponentProperties

DECLARE_DELEGATE_OneParam(FOnComponentPropertiesRegistered, FFlecsComponentProperties);

struct UNREALFLECS_API FFlecsComponentPropertiesRegistry final
{
	static FFlecsComponentPropertiesRegistry Instance;
public:
	static FFlecsComponentPropertiesRegistry& Get()
	{
		return Instance;
	}

	FORCEINLINE void RegisterComponentProperties(const std::string& Name, const std::vector<flecs::entity_t>& Entities,
		const TArray<FInstancedStruct>& ComponentPropertyStructs, const bool bResetExisting = false)
	{
		UNLOG_CATEGORY_SCOPED(LogFlecsComponentProperties);

		if (bResetExisting)
		{
			if (ComponentProperties.contains(Name))
			{
				ComponentProperties.erase(Name);
			}
		}
		
		if (!ComponentProperties.contains(Name))
		{
			ComponentProperties[Name] = FFlecsComponentProperties{ Name, Entities,
				ComponentPropertyStructs };
			
			UN_LOGF(LogFlecsComponentProperties,
				Log,
				"Registered component properties: %s",
				*FString(Name.data()));
		}
		else
		{
			for (const flecs::entity_t& Entity : Entities)
			{
				if (std::ranges::find(ComponentProperties[Name].Entities, Entity)
					!= ComponentProperties[Name].Entities.end())
				{
					continue;
				}

				ComponentProperties[Name].Entities.emplace_back(Entity);

				UN_LOGF(LogFlecsComponentProperties, Log,
					"Updated component properties: %s", *FString(Name.data()));
			}

			for (const FInstancedStruct& ComponentPropertyStruct : ComponentPropertyStructs)
			{
				ComponentProperties[Name].ComponentPropertyStructs.Add(ComponentPropertyStruct);
			}
		}

		OnComponentPropertiesRegistered.ExecuteIfBound(ComponentProperties[Name]);
	}

	FORCEINLINE NO_DISCARD bool ContainsComponentProperties(const std::string& Name) const
	{
		return ComponentProperties.contains(Name);
	}

	FORCEINLINE const FFlecsComponentProperties* GetComponentProperties(const std::string& Name) const
	{
		checkf(!Name.empty(), TEXT("Component properties name is empty!"));
		checkf(ComponentProperties.contains(Name), TEXT("Component properties not found!"));
		return &ComponentProperties.at(Name);
	}

	FORCEINLINE static void RegisterStructMetaData(UScriptStruct* ScriptStruct, const FString& Tags = FString())
	{
		solid_check(ScriptStruct != nullptr);

		const FName MetaDataKey = "FlecsTags";

		if (ScriptStruct->HasMetaData(MetaDataKey))
		{
			const FString ExistingTags = ScriptStruct->GetMetaData(MetaDataKey);
			ScriptStruct->SetMetaData(MetaDataKey, *FString::Printf(TEXT("%s %s"), *ExistingTags, *Tags));
		}
		else
		{
			ScriptStruct->SetMetaData(MetaDataKey, *Tags);
		}
	}
	
	robin_hood::unordered_flat_map<std::string, FFlecsComponentProperties> ComponentProperties;

	FOnComponentPropertiesRegistered OnComponentPropertiesRegistered;
}; // struct FFlecsComponentPropertiesRegistry

/**
 * Do not use this macro directly, use REGISTER_COMPONENT_TAG_PROPERTIES or REGISTER_COMPONENT_TRAIT_PROPERTIES
 */
#define _REGISTER_FLECS_PROPERTIES_TAGS_IMPL(ComponentType, ...) \
	namespace \
	{ \
		struct FAutoRegister##ComponentType##_Tags \
		{ \
			FAutoRegister##ComponentType##_Tags() \
			{ \
				std::vector<flecs::entity_t> Entities = { __VA_ARGS__ } ; \
				FFlecsComponentPropertiesRegistry::Get().RegisterComponentProperties(#ComponentType, Entities, {}); \
				if constexpr (Solid::IsStaticStruct<ComponentType>()) \
				{ \
					FCoreDelegates::OnPostEngineInit.AddLambda([&]() \
					{ \
						UScriptStruct* ScriptStruct = TBaseStructure<ComponentType>::Get(); \
						const FString Tags = TEXT(#__VA_ARGS__); \
						FFlecsComponentPropertiesRegistry::Get().RegisterStructMetaData(ScriptStruct, Tags); \
					}); \
				} \
			} \
		}; \
		inline FAutoRegister##ComponentType##_Tags AutoRegister##ComponentType##_Instance_Tags; \
	}

#define _REGISTER_FLECS_PROPERTIES_TRAITS_IMPL(ComponentType, ...) \
	namespace \
	{ \
		struct FAutoRegister##ComponentType##_Traits \
		{ \
			FAutoRegister##ComponentType##_Traits() \
			{ \
				FCoreDelegates::OnPostEngineInit.AddLambda([&]() \
				{ \
					TArray<FInstancedStruct> ComponentPropertyStructs = { __VA_ARGS__ }; \
					FFlecsComponentPropertiesRegistry::Get().RegisterComponentProperties(#ComponentType, {}, ComponentPropertyStructs); \
				}); \
			} \
		}; \
		inline FAutoRegister##ComponentType##_Traits AutoRegister##ComponentType##_Instance_Traits; \
	}

// @Deprecated
#define REGISTER_FLECS_PROPERTIES(ComponentType, EntitiesArray, ComponentPropertyStructsArray) \
	static_assert(false, \
		"Do not use REGISTER_FLECS_PROPERTIES directly! Use REGISTER_COMPONENT_TAG_PROPERTIES or REGISTER_COMPONENT_TRAIT_PROPERTIES instead.")

#define REGISTER_COMPONENT_TAG_PROPERTIES(ComponentType, ...) \
	_REGISTER_FLECS_PROPERTIES_TAGS_IMPL(ComponentType, __VA_ARGS__ )

// @TODO: Only Support ScriptStructs for now
#define REGISTER_COMPONENT_TRAIT_PROPERTIES(ComponentType, ...) \
	_REGISTER_FLECS_PROPERTIES_TRAITS_IMPL(ComponentType, __VA_ARGS__ )
