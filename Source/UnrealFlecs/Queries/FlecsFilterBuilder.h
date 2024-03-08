﻿// Solstice Games © 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "FlecsFilter.h"
#include "Macros.h"
#include "Entities/FlecsEntityHandle.h"
#include "Entities/FlecsId.h"
#include "FlecsFilterBuilder.generated.h"

USTRUCT(BlueprintType)
struct FFlecsFilterBuilder
{
    GENERATED_BODY()

public:
    FORCEINLINE FFlecsFilterBuilder()
    {
    }
    
    FORCEINLINE explicit FFlecsFilterBuilder(const flecs::world& InWorld, const FString& InName = TEXT(""))
        : FilterBuilder(InWorld, TCHAR_TO_ANSI(*InName))
    {
    }

    FORCEINLINE NO_DISCARD flecs::filter_builder<>& GetFlecsFilterBuilder() { return FilterBuilder; }
    FORCEINLINE NO_DISCARD const flecs::filter_builder<>& GetFlecsFilterBuilder() const { return FilterBuilder; }

    FORCEINLINE operator flecs::filter_builder<>&() { return GetFlecsFilterBuilder(); }
    FORCEINLINE operator const flecs::filter_builder<>&() const { return GetFlecsFilterBuilder(); }

    FORCEINLINE NO_DISCARD flecs::filter_builder<>* operator->() { return &FilterBuilder; }
    FORCEINLINE NO_DISCARD const flecs::filter_builder<>* operator->() const { return &FilterBuilder; }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Filter()
    {
        FilterBuilder.filter();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsId& InId)
    {
        FilterBuilder.term(InId.GetFlecsId());
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsId& InId, const FFlecsId& InRelation)
    {
        FilterBuilder.term(InId.GetFlecsId(), InRelation.GetFlecsId());
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FString& InName)
    {
        FilterBuilder.term(TCHAR_TO_ANSI(*InName));
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FString& InName, const FString& InRelation)
    {
        FilterBuilder.term(TCHAR_TO_ANSI(*InName), TCHAR_TO_ANSI(*InRelation));
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsId& InId, const FString& InName)
    {
        FilterBuilder.term(InId.GetFlecsId(), TCHAR_TO_ANSI(*InName));
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsEntityHandle& InEntity)
    {
        FilterBuilder.term(InEntity);
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsEntityHandle& InEntity, const FFlecsId& InRelation)
    {
        FilterBuilder.term(InEntity, InRelation.GetFlecsId());
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Term(const FFlecsEntityHandle& InEntity, const FString& InName)
    {
        FilterBuilder.term(InEntity, TCHAR_TO_ANSI(*InName));
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& TermAt(const int32 InIndex)
    {
        FilterBuilder.term_at(InIndex);
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& And()
    {
        FilterBuilder.and_();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Or()
    {
        FilterBuilder.or_();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Not()
    {
        FilterBuilder.not_();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Optional()
    {
        FilterBuilder.optional();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& AndFrom()
    {
        FilterBuilder.and_from();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& OrFrom()
    {
        FilterBuilder.or_from();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& NotFrom()
    {
        FilterBuilder.not_from();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Singleton()
    {
        FilterBuilder.singleton();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Entity(const FFlecsEntityHandle& InEntity)
    {
        FilterBuilder.entity(InEntity);
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Instanced()
    {
        FilterBuilder.instanced();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Expression(const FString& InExpression)
    {
        FilterBuilder.expr(TCHAR_TO_ANSI(*InExpression));
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Expression(const char* InExpression)
    {
        FilterBuilder.expr(InExpression);
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Argument(const int32 InIndex)
    {
        FilterBuilder.arg(InIndex);
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder Cascade()
    {
        FilterBuilder.cascade();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Descriptor()
    {
        FilterBuilder.desc();
        return *this;
    }

    FORCEINLINE NO_DISCARD FFlecsFilterBuilder& Name(const FString& InName)
    {
        FilterBuilder.name(TCHAR_TO_ANSI(*InName));
        return *this;
    }

    template <typename FunctionType>
    FORCEINLINE void ForEach(FunctionType Function)
    {
        FilterBuilder.each<FunctionType>(Function);
    }

    FORCEINLINE NO_DISCARD FFlecsFilter Build()
    {
        return FilterBuilder.build();
    }

private:
    flecs::filter_builder<> FilterBuilder;
}; // struct FFlecsFilterBuilder

template <>
struct TStructOpsTypeTraits<FFlecsFilterBuilder> : public TStructOpsTypeTraitsBase2<FFlecsFilterBuilder>
{
    enum
    {
        WithCopy = false,
    }; // enum
    
}; // struct TStructOpsTypeTraits<FFlecsFilterBuilder>
