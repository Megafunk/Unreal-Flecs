﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Entities/FlecsEntityHandle.h"
#include "Entities/FlecsEntityRecord.h"
#include "Interfaces/FlecsEntityInterface.h"
#include "FlecsEntityActorComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Flecs), meta=(BlueprintSpawnableComponent,
	DisplayName = "Flecs Entity Actor Component"))
class UNREALFLECS_API UFlecsEntityActorComponent : public UActorComponent, public IFlecsEntityInterface
{
	GENERATED_BODY()

public:
	UFlecsEntityActorComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	virtual void InitializeEntity();

	FORCEINLINE virtual FFlecsEntityHandle GetEntityHandle() const override final
	{
		return EntityHandle;
	}

	UFUNCTION(BlueprintCallable, Category = "Flecs | Entity")
	FORCEINLINE void SetEntityHandle(const FFlecsEntityHandle& InEntityHandle);

	UPROPERTY(Replicated)
	FFlecsEntityHandle EntityHandle;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Flecs | Entity")
	FFlecsEntityRecord EntityRecord;

	#if WITH_EDITORONLY_DATA

	virtual bool CanEditChange(const FProperty* InProperty) const override;

	#endif // WITH_EDITORONLY_DATA

	UFUNCTION()
	void OnWorldCreated(UFlecsWorld* InWorld);

private:
	void CreateActorEntity(UFlecsWorld* InWorld);
	
}; // class UFlecsEntityActorComponent