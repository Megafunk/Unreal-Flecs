﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/FlecsEntityHandle.h"
#include "UObject/Interface.h"
#include "FlecsEntityInterface.generated.h"

UINTERFACE(BlueprintType)
class UFlecsEntityInterface : public UInterface
{
	GENERATED_BODY()
}; // class UFlecsEntityInterface

class UNREALFLECS_API IFlecsEntityInterface
{
	GENERATED_BODY()

public:

	NO_DISCARD virtual FFlecsEntityHandle GetEntityHandle() const
		PURE_VIRTUAL(IFlecsEntityInterface::GetEntityHandle, return FFlecsEntityHandle(););

}; // class IFlecsEntityInterface
