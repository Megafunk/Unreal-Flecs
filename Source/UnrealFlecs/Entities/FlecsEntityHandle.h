﻿// Solstice Games © 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "FlecsType.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "SolidMacros/Macros.h"
#include "SolidMacros/Concepts/SolidConcepts.h"
#include "FlecsEntityHandle.generated.h"

class UFlecsWorld;

USTRUCT(BlueprintType)
struct UNREALFLECS_API FFlecsEntityHandle
{
	GENERATED_BODY()

	FORCEINLINE NO_DISCARD friend uint32 GetTypeHash(const FFlecsEntityHandle& InEntity)
	{
		return GetTypeHash(InEntity.GetEntity().id());
	}

	FORCEINLINE static NO_DISCARD FFlecsEntityHandle GetNullHandle()
	{
		return flecs::entity::null();
	}

public:
	FFlecsEntityHandle();
	FORCEINLINE FFlecsEntityHandle(const flecs::entity& InEntity) : Entity(InEntity) {}

	FORCEINLINE FFlecsEntityHandle(const flecs::entity_t& InEntity) : Entity(InEntity) 
	{
		FFlecsEntityHandle();
	}
	
	FORCEINLINE NO_DISCARD flecs::entity GetEntity() const { return Entity; }

	FORCEINLINE void SetEntity(const flecs::entity& InEntity) { Entity = InEntity; }
	FORCEINLINE void SetEntity(const flecs::entity_t& InEntity)
	{
		*this = FFlecsEntityHandle(InEntity);
	}
	
	FORCEINLINE operator flecs::entity() const { return GetEntity(); }
	FORCEINLINE operator flecs::id_t() const { return GetEntity().id(); }
	FORCEINLINE operator flecs::entity_view() const { return GetEntity().view(); }
	
	FORCEINLINE NO_DISCARD bool IsValid() const { return GetEntity().is_valid(); }
	FORCEINLINE NO_DISCARD bool IsAlive() const { return GetEntity().is_alive(); }

	FORCEINLINE operator bool() const { return IsValid(); }

	FORCEINLINE NO_DISCARD uint32 GetId() const { return GetEntity().id(); }
	FORCEINLINE NO_DISCARD uint32 GetGeneration() const { return get_generation(GetEntity()); }
	
	FORCEINLINE NO_DISCARD UFlecsWorld* GetFlecsWorld() const;
	FORCEINLINE NO_DISCARD UWorld* GetOuterWorld() const;
	
	FORCEINLINE NO_DISCARD FName GetWorldName() const;
	
	FORCEINLINE NO_DISCARD FFlecsType GetType() const { return FFlecsType(GetEntity().type()); }

	FORCEINLINE NO_DISCARD bool Has(const FFlecsEntityHandle& InEntity) const { return GetEntity().has(InEntity); }

	template <typename T>
	FORCEINLINE NO_DISCARD bool Has() const { return GetEntity().has<T>(); }

	template <typename First, typename Second>
	FORCEINLINE NO_DISCARD bool Has() const { return GetEntity().has<First, Second>(); }

	template <typename TSecond>
	FORCEINLINE NO_DISCARD bool Has(const FFlecsEntityHandle& InFirst) const { return GetEntity().has<TSecond>(InFirst); }

	FORCEINLINE NO_DISCARD bool Has(const UScriptStruct* StructType) const;

	FORCEINLINE NO_DISCARD bool Has(const FGameplayTag& InTag) const;

	FORCEINLINE void Add(const FFlecsEntityHandle& InEntity) const { GetEntity().add(InEntity); }

	FORCEINLINE void Add(const UScriptStruct* StructType) const;

	FORCEINLINE void Add(const FGameplayTag& InTag) const;
	
	template <typename T>
	FORCEINLINE void Add() const { GetEntity().add<T>(); }
	
	FORCEINLINE void Remove(const FFlecsEntityHandle& InEntity) const { GetEntity().remove(InEntity); }

	FORCEINLINE void Remove(const FFlecsEntityHandle InFirst, const FFlecsEntityHandle InSecond) const
	{
		GetEntity().remove(InFirst, InSecond);
	}

	FORCEINLINE void Remove(const UScriptStruct* StructType) const;

	FORCEINLINE void Remove(const FGameplayTag& InTag) const;

	template <typename T>
	FORCEINLINE void Remove() const { GetEntity().remove<T>(); }

	FORCEINLINE void Set(const FFlecsEntityHandle& InEntity) const { GetEntity().set(InEntity); }

	template <typename T>
	FORCEINLINE void Set(const T& InValue) const { GetEntity().set<T>(InValue); }

	FORCEINLINE void Set(const FFlecsEntityHandle& InEntity, const void* InValue) const
	{
		GetEntity().set_ptr(InEntity, InValue);
	}

	FORCEINLINE void Set(const UScriptStruct* StructType, const void* InValue) const;

	FORCEINLINE void Set(const FInstancedStruct& InValue) const;
	
	template <typename T>
	FORCEINLINE NO_DISCARD T Get() const { return GetEntity().get<T>(); }

	FORCEINLINE NO_DISCARD const void* GetPtr(const FFlecsEntityHandle& InEntity) const { return GetEntity().get(InEntity); }

	template <typename T>
	FORCEINLINE NO_DISCARD T* GetPtr() { return GetEntity().get_mut<T>(); }
	
	template <typename T>
	FORCEINLINE NO_DISCARD const T* GetPtr() const { return GetEntity().get<T>(); }

	FORCEINLINE NO_DISCARD const void* GetPtr(const UScriptStruct* StructType) const;

	template <typename T>
	FORCEINLINE NO_DISCARD T& GetRef() { return *GetEntity().get_ref<T>().get(); }

	template <typename T>
	FORCEINLINE NO_DISCARD const T& GetRef() const { return *GetEntity().get_ref<T>().get(); }

	template <typename T>
	FORCEINLINE NO_DISCARD bool Has() { return GetEntity().has<T>(); }

	FORCEINLINE void Clear() const { GetEntity().clear(); }

	FORCEINLINE void Enable() const { GetEntity().enable(); }
	FORCEINLINE void Disable() const { GetEntity().disable(); }

	template <typename T>
	FORCEINLINE void Enable() const { GetEntity().enable<T>(); }

	FORCEINLINE void Enable(const FFlecsEntityHandle& InEntity) const { GetEntity().enable(InEntity); }
	FORCEINLINE void Enable(const UScriptStruct* StructType) const;

	FORCEINLINE void Disable(const FFlecsEntityHandle& InEntity) const { GetEntity().disable(InEntity); }
	FORCEINLINE void Disable(const UScriptStruct* StructType) const;

	template <typename T>
	FORCEINLINE void Disable() const { GetEntity().disable<T>(); }

	FORCEINLINE void Toggle() const { GetEntity().enable(!IsEnabled()); }

	FORCEINLINE NO_DISCARD bool IsEnabled() const { return GetEntity().enabled(); }

	template <typename T>
	FORCEINLINE NO_DISCARD bool IsEnabled() const { return GetEntity().enabled<T>(); }

	FORCEINLINE NO_DISCARD bool IsEnabled(const FFlecsEntityHandle& InEntity) const { return GetEntity().enabled(InEntity); }
	FORCEINLINE NO_DISCARD bool IsEnabled(const UScriptStruct* StructType) const;

	FORCEINLINE void Destroy() const { GetEntity().destruct(); }

	FORCEINLINE NO_DISCARD FFlecsEntityHandle Clone(const bool bCloneValue = true, const int32 DestinationId = 0) const
	{
		return GetEntity().clone(bCloneValue, DestinationId);
	}

	FORCEINLINE void SetName(const FString& InName) const { GetEntity().set_name(TCHAR_TO_ANSI(*InName)); }
	FORCEINLINE NO_DISCARD FString GetName() const { return FString(GetEntity().name()); }

	FORCEINLINE void SetDocBrief(const FString& InDocBrief) const { GetEntity().set_doc_brief(TCHAR_TO_ANSI(*InDocBrief)); }
	FORCEINLINE NO_DISCARD FString GetDocBrief() const { return FString(GetEntity().doc_brief()); }

	FORCEINLINE void SetDocColor(const FString& Link) const { GetEntity().set_doc_color(TCHAR_TO_ANSI(*Link)); }
	FORCEINLINE NO_DISCARD FString GetDocColor() const { return FString(GetEntity().doc_color()); }

	FORCEINLINE void SetDocName(const FString& InDocName) const { GetEntity().set_doc_name(TCHAR_TO_ANSI(*InDocName)); }
	FORCEINLINE NO_DISCARD FString GetDocName() const { return FString(GetEntity().doc_name()); }

	FORCEINLINE void SetDocLink(const FString& InDocLink) const { GetEntity().set_doc_link(TCHAR_TO_ANSI(*InDocLink)); }
	FORCEINLINE NO_DISCARD FString GetDocLink() const { return FString(GetEntity().doc_link()); }

	FORCEINLINE void SetDocDetails(const FString& InDocDetails) const { GetEntity().set_doc_detail(TCHAR_TO_ANSI(*InDocDetails)); }
	FORCEINLINE NO_DISCARD FString GetDocDetails() const { return FString(GetEntity().doc_detail()); }

	FORCEINLINE void SetPair(const FFlecsEntityHandle& InRelation, const FFlecsEntityHandle& InTarget) const
	{
		GetEntity().set_second(InRelation, InTarget);
	}

	FORCEINLINE NO_DISCARD bool IsPair() const
	{
		return GetEntity().is_pair();
	}
	
	FORCEINLINE NO_DISCARD FFlecsEntityHandle GetParent() const
	{
		return GetEntity().parent();
	}

	FORCEINLINE void SetParent(const FFlecsEntityHandle& InParent, const bool bIsA = false) const
	{
		GetEntity().child_of(InParent);

		if (bIsA)
		{
			GetEntity().is_a(InParent);
		}
	}

	FORCEINLINE NO_DISCARD bool IsPrefab() const
	{
		return Has(flecs::Prefab);
	}

	FORCEINLINE NO_DISCARD bool IsComponent() const
	{
		return Has<flecs::Component>() || Has(flecs::IsA);
	}

	FORCEINLINE NO_DISCARD flecs::untyped_component& GetUntypedComponent()
	{
		solid_checkf(IsComponent(), TEXT("Entity is not a component"));
		return GetRef<flecs::untyped_component>();
	}

	FORCEINLINE NO_DISCARD const flecs::untyped_component* GetUntypedComponent() const
	{
		solid_checkf(IsComponent(), TEXT("Entity is not a component"));
		return GetPtr<flecs::untyped_component>();
	}

	template <typename T>
	FORCEINLINE void Emit() const
	{
		GetEntity().emit<T>();
	}

	template <typename T>
	FORCEINLINE void Emit(const T& InValue) const
	{
		GetEntity().emit<T>(InValue);
	}

	FORCEINLINE void Emit(const FFlecsEntityHandle& InEntity) const
	{
		GetEntity().emit(InEntity);
	}

	template <typename FunctionType>
	FORCEINLINE void Observe(FunctionType&& InFunction) const
	{
		GetEntity().observe(InFunction);
	}

	template <typename TEvent, typename FunctionType>
	FORCEINLINE void Observe(FunctionType&& InFunction) const
	{
		GetEntity().observe<TEvent>(InFunction);
	}

	template <typename FunctionType>
	FORCEINLINE void Observe(const FFlecsEntityHandle& InEntity, FunctionType&& InFunction) const
	{
		GetEntity().observe(InEntity, InFunction);
	}

	FORCEINLINE NO_DISCARD bool operator==(const FFlecsEntityHandle& Other) const
	{
		return GetEntity() == Other.GetEntity();
	}
	
	FORCEINLINE NO_DISCARD bool operator!=(const FFlecsEntityHandle& Other) const
	{
		return GetEntity() != Other.GetEntity();
	}

	FORCEINLINE NO_DISCARD bool operator==(const flecs::entity& Other) const
	{
		return GetEntity() == Other;
	}

	FORCEINLINE NO_DISCARD bool operator!=(const flecs::entity& Other) const
	{
		return GetEntity() != Other;
	}

	FORCEINLINE flecs::entity* operator->() { return &Entity; }
	FORCEINLINE const flecs::entity* operator->() const { return &Entity; }

	FORCEINLINE NO_DISCARD FString ToString() const
	{
		return FString::Printf(TEXT("Entity: %hs"), GetEntity().str().c_str());
	}

	template <typename TEnum>
	FORCEINLINE NO_DISCARD TEnum ToConstant() const
	{
		return GetEntity().to_constant<TEnum>();
	}

	FORCEINLINE NO_DISCARD FString ToJson(const bool bSerializePath = true,
		const bool bSerializeLabel = false, const bool bSerializeBrief = false, const bool bSerializeLink = false,
		const bool bSerializeColor = false, const bool bSerializeIds = true, const bool bSerializeIdLabels = false,
		const bool bSerializeBaseComponents = true, const bool bSerializeComponents = true)
	{
		return FString(GetEntity().to_json().c_str());
	}

	FORCEINLINE void FromJson(const FString& InJson) const
	{
		GetEntity().from_json(TCHAR_TO_ANSI(*InJson));
	}

	FORCEINLINE bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		if (Ar.IsLoading())
		{
			FString Json;
			Ar << Json;
			FromJson(Json);
		}
		else
		{
			FString Json = ToJson();
			Ar << Json;
		}
		
		SerializeOptionalValue(Ar.IsSaving(), Ar, WorldName, FName("DefaultFlecsWorld"));
			
		bOutSuccess = true;
		return true;
	}

	template <typename FunctionType>
	FORCEINLINE void Iterate(const FunctionType& InFunction) const
	{
		GetEntity().each(InFunction);
	}

	template <typename TFirst, typename FunctionType>
	FORCEINLINE void Iterate(const FunctionType& InFunction) const
	{
		GetEntity().each<TFirst, FunctionType>(InFunction);
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE void AddTrait() const
	{
		GetEntity().add<TComponent, TTrait>();
	}

	template <typename TComponent>
	FORCEINLINE void AddTrait(const UScriptStruct* StructType) const
	{
		GetEntity().add<TComponent>(StructType);
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE void RemoveTrait() const
	{
		GetEntity().remove<TComponent, TTrait>();
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE bool HasTrait() const
	{
		return GetEntity().has<TComponent, TTrait>();
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE void SetTrait(const TTrait& InValue) const
	{
		GetEntity().set<TComponent, TTrait>(InValue);
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE TTrait GetTrait() const
	{
		return GetEntity().get<TComponent, TTrait>();
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE TTrait* GetTraitPtr() const
	{
		return GetEntity().get_mut<TComponent, TTrait>();
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE TTrait& GetTraitRef() const
	{
		return *GetEntity().get_ref<TComponent, TTrait>().get();
	}

	template <typename TComponent, typename TTrait>
	FORCEINLINE void EmitTrait() const
	{
		GetEntity().emit<TComponent, TTrait>();
	}

	#if WITH_EDITORONLY_DATA

	UPROPERTY()
	FName DisplayName;

	#endif // WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Category = "Flecs")
	FName WorldName = FName("DefaultFlecsWorld");
	
private:
	flecs::entity Entity;

	FORCEINLINE NO_DISCARD FFlecsEntityHandle ObtainComponentTypeStruct(const UScriptStruct* StructType) const;
	FORCEINLINE NO_DISCARD FFlecsEntityHandle GetTagEntity(const FGameplayTag& InTag) const;
	
}; // struct FFlecsEntityHandle

template <>
struct TStructOpsTypeTraits<FFlecsEntityHandle> : public TStructOpsTypeTraitsBase2<FFlecsEntityHandle>
{
	enum
	{
		WithNetSerializer = true,
	}; // enum
	
}; // struct TStructOpsTypeTraits<FFlecsEntityHandle>
