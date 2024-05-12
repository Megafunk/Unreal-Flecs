﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "Entities/FlecsEntityHandle.h"
#include "FlecsEntityRecord.generated.h"

UENUM(BlueprintType)
enum class EFlecsComponentNodeType : uint8
{
	ScriptStruct = 0,
	EntityHandle = 1,
	FGameplayTag = 2,
}; // enum class EFlecsComponentNodeType


USTRUCT(BlueprintType)
struct UNREALFLECS_API FFlecsTraitTypeInfo final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree")
	EFlecsComponentNodeType NodeType = EFlecsComponentNodeType::ScriptStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::ScriptStruct", EditConditionHides))
	FInstancedStruct ScriptStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::EntityHandle", EditConditionHides))
	FFlecsEntityHandle EntityHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::FGameplayTag", EditConditionHides))
	FGameplayTag GameplayTag;
	
}; // struct FFlecsTraitTypeInfo

USTRUCT(BlueprintType)
struct UNREALFLECS_API FFlecsComponentTypeInfo final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree")
	EFlecsComponentNodeType NodeType = EFlecsComponentNodeType::ScriptStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::ScriptStruct", EditConditionHides))
	FInstancedStruct ScriptStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::EntityHandle", EditConditionHides))
	FFlecsEntityHandle EntityHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree",
		meta = (EditCondition = "NodeType == EFlecsComponentTreeNodeType::FGameplayTag", EditConditionHides))
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Component Tree")
	TArray<FFlecsTraitTypeInfo> Traits;
	
}; // struct FFlecsComponentTypeInfo

USTRUCT(BlueprintType)
struct UNREALFLECS_API FFlecsEntityRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Entity Record")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs | Entity Record")
	TArray<FFlecsComponentTypeInfo> Components;

	FORCEINLINE void ApplyRecordToEntity(const FFlecsEntityHandle& InEntityHandle) const
	{
		if (!Name.IsEmpty())
		{
			InEntityHandle.SetName(Name);
		}

		for (const auto& [NodeType, ScriptStruct, EntityHandle, GameplayTag, Traits] : Components)
		{
			switch (NodeType)
			{
			case EFlecsComponentNodeType::ScriptStruct:
				InEntityHandle.Set(ScriptStruct);
				break;
			case EFlecsComponentNodeType::EntityHandle:
				InEntityHandle.Add(EntityHandle);
				break;
			case EFlecsComponentNodeType::FGameplayTag:
				InEntityHandle.Add(GameplayTag);
				break;
			default: UNLIKELY_ATTRIBUTE
				checkNoEntry();
				break;
			}
		}
	}

}; // struct FFlecsEntityRecord