﻿// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppRedundantTemplateArguments
#pragma once

#include "CoreMinimal.h"
#include "SolidMacros/Macros.h"
#include "UObject/Object.h"
#include "FlecsUObjectComponent.generated.h"

USTRUCT(BlueprintType)
struct UNREALFLECS_API FFlecsUObjectComponent final
{
	GENERATED_BODY()

	FORCEINLINE NO_DISCARD friend uint32 GetTypeHash(const FFlecsUObjectComponent& InComponent)
	{
		return GetTypeHash(InComponent.Object);
	}

public:
	FORCEINLINE FFlecsUObjectComponent() = default;
	FORCEINLINE FFlecsUObjectComponent(UObject* InObject) : Object(InObject) {}

	FORCEINLINE NO_DISCARD UObject* GetObject() const { return Object.Get(); }

	template <typename T>
	FORCEINLINE NO_DISCARD T* GetObject() const { return Cast<T>(GetObject()); }

	template <typename T>
	FORCEINLINE NO_DISCARD T* GetObjectChecked() const { return CastChecked<T>(GetObject()); }
	
	FORCEINLINE void SetObject(UObject* InObject) { Object = InObject; }

	template <typename T>
	FORCEINLINE void SetObject(T* InObject) { Object = InObject; }

	FORCEINLINE NO_DISCARD bool IsValid() const { return Object.IsValid(false, true); }
	FORCEINLINE operator bool() const { return IsValid(); }
	
	FORCEINLINE NO_DISCARD bool IsStale() const { return Object.IsStale(); }

	FORCEINLINE void Reset() { Object.Reset(); }

	FORCEINLINE bool operator==(const FFlecsUObjectComponent& Other) const
	{
		return Object == Other.Object;
	}

	FORCEINLINE bool operator!=(const FFlecsUObjectComponent& Other) const
	{
		return !(*this == Other);
	}
	
	UPROPERTY()
	TWeakObjectPtr<UObject> Object;

	FORCEINLINE NO_DISCARD FString ToString() const
	{
		return Object.IsValid() ? Object->GetName() : TEXT("Invalid");
	}
	
}; // struct FFlecsUObjectComponent