﻿// Solstice Games © 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Standard/Hashing.h"
#include "flecs.h"
#include "Properties/FlecsComponentProperties.h"
#include "FlecsScriptClassComponent.generated.h"

USTRUCT(BlueprintType)
struct FFlecsScriptClassComponent
{
    GENERATED_BODY()

    FORCEINLINE NO_DISCARD friend uint32 GetTypeHash(const FFlecsScriptClassComponent& InScriptClassComponent)
    {
        return GetTypeHash(InScriptClassComponent.ScriptClass);
    }

    FORCEINLINE NO_DISCARD friend bool operator==(const FFlecsScriptClassComponent& Lhs, const FFlecsScriptClassComponent& Rhs)
    {
        return Lhs.ScriptClass == Rhs.ScriptClass;
    }

    FORCEINLINE NO_DISCARD friend bool operator!=(const FFlecsScriptClassComponent& Lhs, const FFlecsScriptClassComponent& Rhs)
    {
        return !(Lhs == Rhs);
    }

    FORCEINLINE operator TSubclassOf<UObject>() const { return ScriptClass.Get(); }
    
    FORCEINLINE FFlecsScriptClassComponent(const TSubclassOf<UObject>& InScriptClass = nullptr) : ScriptClass(InScriptClass) {}

    template <typename T>
    FORCEINLINE NO_DISCARD TSubclassOf<T> Get() const
    {
        return Cast<T>(ScriptClass.Get());
    }

    template <typename T>
    FORCEINLINE NO_DISCARD TSubclassOf<T> GetChecked() const
    {
        return CastChecked<T>(ScriptClass.Get());
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flecs")
    TSubclassOf<UObject> ScriptClass;
    
}; // struct FFlecsScriptClassComponent

REGISTER_COMPONENT_TAG_PROPERTIES(FFlecsScriptClassComponent, flecs::Sparse)

DEFINE_STD_HASH(FFlecsScriptClassComponent)

