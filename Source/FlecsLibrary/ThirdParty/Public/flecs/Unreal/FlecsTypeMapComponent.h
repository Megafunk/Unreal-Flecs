﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsScriptStructComponent.h"
#include "FlecsScriptClassComponent.h"
#include "FlecsScriptEnumComponent.h"
#include "Standard/robin_hood.h"

struct FFlecsTypeMapComponent final
{
	mutable robin_hood::unordered_flat_map<FFlecsScriptStructComponent, flecs::entity> ScriptStructMap;
	mutable robin_hood::unordered_flat_map<FFlecsScriptClassComponent, flecs::entity> ScriptClassMap;
	mutable robin_hood::unordered_flat_map<FFlecsScriptEnumComponent, flecs::entity> ScriptEnumMap;
}; // struct FFlecsTypeMapComponent
