﻿// Solstice Games © 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Standard/Hashing.h"
#include "FlecsScriptStructComponent.generated.h"

USTRUCT(BlueprintType)
struct FFlecsScriptStructComponent
{
    GENERATED_BODY()

    FORCEINLINE NO_DISCARD friend uint32 GetTypeHash(const FFlecsScriptStructComponent& InScriptStructComponent)
    {
        return GetTypeHash(InScriptStructComponent.ScriptStruct);
    }

    FORCEINLINE NO_DISCARD friend bool operator==(const FFlecsScriptStructComponent& Lhs, const FFlecsScriptStructComponent& Rhs)
    {
        return Lhs.ScriptStruct == Rhs.ScriptStruct;
    }

    FORCEINLINE NO_DISCARD friend bool operator!=(const FFlecsScriptStructComponent& Lhs, const FFlecsScriptStructComponent& Rhs)
    {
        return !(Lhs == Rhs);
    }

    FORCEINLINE operator UScriptStruct*() const { return ScriptStruct.Get(); }
    
    FORCEINLINE FFlecsScriptStructComponent(UScriptStruct* InScriptStruct = nullptr) : ScriptStruct(InScriptStruct) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs")
    TObjectPtr<UScriptStruct> ScriptStruct;
    
}; // struct FFlecsScriptStructComponent

DEFINE_STD_HASH(FFlecsScriptStructComponent)
