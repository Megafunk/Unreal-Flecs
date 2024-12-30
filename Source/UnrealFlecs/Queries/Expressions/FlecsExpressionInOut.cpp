﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FlecsExpressionInOut.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlecsExpressionInOut)

FFlecsExpressionInOut::FFlecsExpressionInOut() : Super(false)
{
}

void FFlecsExpressionInOut::Apply(UFlecsWorld* InWorld, flecs::query_builder<>& InQueryBuilder) const
{
	InQueryBuilder.inout_stage(static_cast<flecs::inout_kind_t>(InOut));
}
