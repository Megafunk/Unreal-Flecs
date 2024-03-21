﻿// Solstice Games © 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "Components/FlecsTypeMapComponent.h"
#include "SolidMacros/Concepts/SolidConcepts.h"
#include "Entities/FlecsId.h"
#include "Systems/FlecsSystem.h"
#include "FlecsWorld.generated.h"

UCLASS(BlueprintType)
class UNREALFLECS_API UFlecsWorld : public UObject
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetWorld(flecs::world&& InWorld)
	{
		World = std::move(InWorld);
	}
	
	FORCEINLINE void Reset()
	{
		World.reset();
	}

	FORCEINLINE void ResetClock() const
	{
		World.reset_clock();
	}

	template <typename ...TArgs>
	FORCEINLINE FFlecsEntityHandle CreateEntity(const TArgs&... Args) const
	{
		return World.entity(Args...);
	}

	template <typename FunctionType>
	FORCEINLINE void ForEach(FunctionType&& Function) const
	{
		World.each(std::forward<FunctionType>(Function));
	}

	template <typename T, typename FunctionType>
	FORCEINLINE void ForEach(FunctionType&& Function) const
	{
		World.each<T>(std::forward<FunctionType>(Function));
	}

	template <typename FunctionType>
	FORCEINLINE void ForEach(const FFlecsId& InTermId, FunctionType&& Function) const
	{
		World.each(InTermId.GetFlecsId(), std::forward<FunctionType>(Function));
	}

	template <typename T>
	FORCEINLINE void AddSingleton() const
	{
		World.add<T>();
	}

	template <typename T>
	FORCEINLINE void SetSingleton(const T& Value) const
	{
		World.set<T>(Value);
	}

	template <typename First, typename Second>
	FORCEINLINE void SetSingleton(const First& InFirst, const Second InSecond) const
	{
		World.set<First, Second>(InSecond, InFirst);
	}

	template <typename First, typename ...TArgs>
	FORCEINLINE void SetSingleton(const First& InFirst, const TArgs&... Args) const
	{
		World.set(std::forward<const TArgs&>(Args)..., InFirst);
	}

	template <typename T>
	FORCEINLINE void RemoveSingleton() const
	{
		World.remove<T>();
	}

	template <typename First, typename Second>
	FORCEINLINE void RemoveSingleton() const
	{
		World.remove<First, Second>();
	}

	template <typename First, typename Second>
	FORCEINLINE void RemoveSingleton(const Second& InSecond) const
	{
		World.remove<First>(InSecond);
	}
	
	template <typename First>
	FORCEINLINE void RemoveSingleton(const FFlecsId& InSecond) const
	{
		World.remove<First>(InSecond);
	}

	template <typename First, typename ...TArgs>
	FORCEINLINE void RemoveSingleton() const
	{
		World.remove<First, TArgs...>();
	}
	
	template <typename T>
	FORCEINLINE NO_DISCARD bool HasSingleton() const
	{
		return World.has<T>();
	}

	template <typename First, typename Second>
	FORCEINLINE NO_DISCARD bool HasSingleton() const
	{
		return World.has<First, Second>();
	}

	template <typename First, typename Second>
	FORCEINLINE NO_DISCARD bool HasSingleton(const Second& InSecond) const
	{
		return World.has<First>(InSecond);
	}
	
	template <typename First>
	FORCEINLINE NO_DISCARD bool HasSingleton(const FFlecsId& InSecond) const
	{
		return World.has<First>(InSecond);
	}

	template <typename First, typename ...TArgs>
	FORCEINLINE NO_DISCARD bool HasSingleton() const
	{
		return World.has<First, TArgs...>();
	}

	template <typename T>
	FORCEINLINE NO_DISCARD T GetSingleton() const
	{
		return *World.get<T>();
	}

	template <typename First, typename Second>
	FORCEINLINE NO_DISCARD First GetSingleton(const Second& InSecond) const
	{
		return *World.get<First, Second>(InSecond);
	}

	template <typename T>
	FORCEINLINE NO_DISCARD const T* GetSingletonPtr() const
	{
		return World.get<T>();
	}

	template <typename T>
	FORCEINLINE NO_DISCARD flecs::ref<T> GetSingletonRef() const
	{
		return World.get_ref<T>();
	}

	FORCEINLINE void Merge() const
	{
		World.merge();
	}

	FORCEINLINE void SetAutoMerge(const bool bInAutoMerge) const
	{
		World.set_automerge(bInAutoMerge);
	}

	FORCEINLINE FName GetWorldName() const
	{
		return GetSingleton<FName>();
	}

	FORCEINLINE void SetName(const FName& InName) const
	{
		SetSingleton<FName>(InName);

		#if WITH_EDITOR

		const char* Name = TCHAR_TO_ANSI(*InName.ToString());

		GetWorldEntity().SetDocName(Name);
		
		World.entity<FName>().set_doc_name(Name);

		#endif // WITH_EDITOR
	}

	template <typename T>
	FORCEINLINE void ImportModule()
	{
		World.import<T>();
	}

	FORCEINLINE bool BeginDefer() const
	{
		return World.defer_begin();
	}

	FORCEINLINE bool EndDefer() const
	{
		return World.defer_end();
	}

	FORCEINLINE bool IsDeferred() const
	{
		return World.is_deferred();
	}

	FORCEINLINE bool BeginReadOnly() const
	{
		return World.readonly_begin();
	}

	FORCEINLINE void EndReadOnly() const
	{
		World.readonly_end();
	}

	FORCEINLINE void SetContext(void* InContext) const
	{
		World.set_ctx(InContext);
	}

	FORCEINLINE bool Progress(const double DeltaTime = 0.0) const
	{
		return World.progress(DeltaTime);
	}

	FORCEINLINE void SetTimeScale(const double InTimeScale) const
	{
		World.set_time_scale(InTimeScale);
	}

	FORCEINLINE void SetTargetFps(const double InTargetFps) const
	{
		World.set_target_fps(InTargetFps);
	}

	FORCEINLINE void Quit() const
	{
		World.quit();
	}

	FORCEINLINE void DestroyWorld() const
	{
		Quit();
	}

	FORCEINLINE void SetPipeline(const FFlecsEntityHandle& InPipeline) const
	{
		World.set_pipeline(InPipeline);
	}

	template <typename T>
	FORCEINLINE void SetPipeline() const
	{
		World.set_pipeline<T>();
	}

	FORCEINLINE FFlecsEntityHandle GetPipeline() const
	{
		return World.get_pipeline();
	}

	FORCEINLINE FFlecsEntityHandle SetScope(const FFlecsEntityHandle& InScope) const
	{
		return World.set_scope(InScope);
	}

	template <typename T>
	FORCEINLINE FFlecsEntityHandle SetScope() const
	{
		return World.set_scope<T>();
	}

	FORCEINLINE FFlecsEntityHandle GetScope() const
	{
		return World.get_scope();
	}

	FORCEINLINE float GetDeltaTime() const
	{
		return World.delta_time();
	}

	FORCEINLINE FFlecsEntityHandle GetAlive(const FFlecsId& InId) const
	{
		return World.get_alive(InId.GetFlecsId());
	}

	FORCEINLINE FFlecsEntityHandle MakeAlive(const FFlecsId& InId) const
	{
		return World.make_alive(InId.GetFlecsId());
	}

	template <typename ...TComponents>
	FORCEINLINE NO_DISCARD FFlecsSystem CreateSystem(const FString& InName) const
	{
		return FFlecsSystem(World.system<TComponents...>(TCHAR_TO_ANSI(*InName)));
	}

	template <typename ...TComponents>
	FORCEINLINE NO_DISCARD flecs::system_builder<TComponents...> CreateSystem(const FString& InName) const
	{
		return World.system<TComponents...>(TCHAR_TO_ANSI(*InName));
	}

	FORCEINLINE FFlecsEntityHandle GetWorldEntity() const
	{
		return World.entity(EcsWorld);
	}

	FORCEINLINE void SetStageCount(const int32 InStageCount) const
	{
		World.set_stage_count(InStageCount);
	}

	FORCEINLINE int32 GetStageCount() const
	{
		return World.get_stage_count();
	}

	FORCEINLINE int32 GetStageId() const
	{
		return World.get_stage_id();
	}

	FORCEINLINE bool IsStage() const
	{
		return World.is_stage();
	}

	FORCEINLINE UFlecsWorld* GetStage(const int32 InStageId) const
	{
		UFlecsWorld* Stage = NewObject<UFlecsWorld>();
		Stage->SetWorld(World.get_stage(InStageId));
		return Stage;
	}

	FORCEINLINE UFlecsWorld* ObtainAsyncStage() const
	{
		UFlecsWorld* Stage = NewObject<UFlecsWorld>();
		Stage->SetWorld(World.async_stage());
		return Stage;
	}

	FORCEINLINE bool IsReadOnly() const
	{
		return World.is_readonly();
	}

	FORCEINLINE void PreallocateEntities(const int32 InEntityCount) const
	{
		World.dim(InEntityCount);
	}

	FORCEINLINE void SetEntityRange(const int32 InMin, const int32 InMax) const
	{
		World.set_entity_range(InMin, InMax);
	}

	FORCEINLINE void EnforceEntityRange(const bool bInEnforce) const
	{
		World.enable_range_check(bInEnforce);
	}

	template <typename FunctionType>
	FORCEINLINE void ForEachChild(FunctionType&& Function) const
	{
		World.children(std::forward<FunctionType>(Function));
	}

	FORCEINLINE void SetThreads(const int32 InThreadCount) const
	{
		World.set_threads(InThreadCount);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs|World")
	FORCEINLINE int32 GetThreads() const
	{
		return World.get_threads();
	}

	FORCEINLINE bool UsingTaskThreads() const
	{
		return World.using_task_threads();
	}

	FORCEINLINE void SetTaskThreads(const int32 InThreadCount) const
	{
		World.set_task_threads(InThreadCount);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FORCEINLINE bool HasScriptStruct(UScriptStruct* ScriptStruct) const
	{
		return GetSingletonRef<FFlecsTypeMapComponent>()->ScriptStructMap.contains(ScriptStruct);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FORCEINLINE bool HasScriptClass(const TSubclassOf<UObject> ScriptClass) const
	{
		return GetSingletonRef<FFlecsTypeMapComponent>()->ScriptClassMap.contains(ScriptClass);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle GetScriptStructEntity(UScriptStruct* ScriptStruct) const
	{
		return GetSingletonRef<FFlecsTypeMapComponent>()->ScriptStructMap.at(ScriptStruct);
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle GetScriptClassEntity(const TSubclassOf<UObject> ScriptClass) const
	{
		return GetSingletonRef<FFlecsTypeMapComponent>()->ScriptClassMap.at(ScriptClass);
	}

	template <Solid::TStaticStructConcept T>
	FORCEINLINE NO_DISCARD FFlecsEntityHandle GetScriptStructEntity() const
	{
		return GetScriptStructEntity(T::StaticStruct());
	}
	
	template <Solid::TStaticClassConcept T>
	FORCEINLINE NO_DISCARD FFlecsEntityHandle GetScriptClassEntity() const
	{
		return GetScriptClassEntity(T::StaticClass());
	}

	template <Solid::TStaticStructConcept T>
	FORCEINLINE NO_DISCARD bool HasScriptStruct() const
	{
		return HasScriptStruct(T::StaticStruct());
	}

	template <Solid::TStaticClassConcept T>
	FORCEINLINE NO_DISCARD bool HasScriptClass() const
	{
		return HasScriptClass(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle RegisterScriptStruct(UScriptStruct* ScriptStruct) const
	{
		if UNLIKELY_IF(ScriptStruct == nullptr)
		{
			return FFlecsEntityHandle();
		}
		
		const flecs::entity ScriptStructComponent
			= World.entity(TCHAR_TO_ANSI(*ScriptStruct->GetFName().ToString()))
			                                         .set<flecs::Component>(
			                                         {
				                                         ScriptStruct->GetStructureSize(),
				                                         ScriptStruct->GetMinAlignment()
			                                         })
			                                         .set<FFlecsScriptStructComponent>(
			                                         {
				                                         ScriptStruct
			                                         });

		const FFlecsEntityHandle Handle(ScriptStructComponent);
		GetSingletonRef<FFlecsTypeMapComponent>()->ScriptStructMap.emplace(ScriptStruct, Handle);

		if (ScriptStruct->GetSuperStruct())
		{
			if (!HasScriptStruct(static_cast<UScriptStruct*>(ScriptStruct->GetSuperStruct())))
			{
				RegisterScriptStruct(static_cast<UScriptStruct*>(ScriptStruct->GetSuperStruct()));
			}
		}
		
		return Handle;
	}

	template <Solid::TStaticStructConcept T>
	FORCEINLINE FFlecsEntityHandle RegisterScriptStruct() const
	{
		return RegisterScriptStruct(T::StaticStruct());
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle RegisterScriptClass(TSubclassOf<UObject> ScriptClass) const
	{
		if UNLIKELY_IF(ScriptClass == nullptr)
		{
			return FFlecsEntityHandle();
		}
		
		const flecs::entity ScriptClassComponent
			= World.entity(TCHAR_TO_ANSI(*ScriptClass->GetFName().ToString()))
			                                        .set<flecs::Component>(
			                                        {
				                                        ScriptClass->GetStructureSize(),
				                                        ScriptClass->GetMinAlignment()
			                                        })
			                                        .set<FFlecsScriptClassComponent>(
			                                        {
				                                        ScriptClass
			                                        });

		const FFlecsEntityHandle Handle(ScriptClassComponent);
		GetSingletonRef<FFlecsTypeMapComponent>()->ScriptClassMap.emplace(ScriptClass, Handle);

		if (ScriptClass->GetSuperClass())
		{
			if (!HasScriptClass(ScriptClass->GetSuperClass()))
			{
				MAYBE_UNUSED FFlecsEntityHandle Entity = RegisterScriptClass(ScriptClass->GetSuperClass());
			}
		}
		
		return Handle;
	}

	template <Solid::TStaticClassConcept T>
	FORCEINLINE FFlecsEntityHandle RegisterScriptClass() const
	{
		return RegisterScriptClass(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle RegisterComponentType(const FName& Name, const int32 Size, const int32 Alignment) const
	{
		const flecs::entity Component = World.entity(TCHAR_TO_ANSI(*Name.ToString()))
			.set<flecs::Component>({ Size, Alignment });

		return FFlecsEntityHandle(Component);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle ObtainComponentTypeStruct(UScriptStruct* ScriptStruct) const
	{
		if (HasScriptStruct(ScriptStruct))
		{
			return GetScriptStructEntity(ScriptStruct);
		}

		return RegisterScriptStruct(ScriptStruct);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Flecs")
	FORCEINLINE FFlecsEntityHandle ObtainComponentTypeClass(const TSubclassOf<UObject> ScriptClass) const
	{
		if (HasScriptClass(ScriptClass))
		{
			return GetScriptClassEntity(ScriptClass);
		}

		return RegisterScriptClass(ScriptClass);
	}
	
	flecs::world World;
	
}; // class UFlecsWorld
