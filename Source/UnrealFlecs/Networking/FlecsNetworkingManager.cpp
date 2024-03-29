﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FlecsNetworkingManager.h"
#include "FlecsNetworkIdComponent.h"
#include "FlecsNetworkingActorComponent.h"
#include "FlecsPushModelTrait.h"
#include "FlecsReplicatedTrait.h"
#include "Unlog/Unlog.h"
#include "Worlds/FlecsWorldSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlecsNetworkingManager)

UNLOG_CATEGORY(LogFlecsNetworkingManager);

UFlecsNetworkingManager::UFlecsNetworkingManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UFlecsNetworkingManager::BeginPlay()
{
	Super::BeginPlay();

	FlecsWorld = UFlecsWorldSubsystem::GetDefaultWorld(this);

	#if WITH_SERVER_CODE

	if (GetOwner()->HasAuthority())
	{
		NetworkIdObserver = FlecsWorld->CreateObserver<FFlecsNetworkIdComponent>(TEXT("NetworkingIdObserver"))
		.event(flecs::OnAdd)
		.yield_existing(true)
		.term(flecs::Name)
			.and_()
			.inout_none()
		.each([this](const FFlecsEntityHandle& Entity, FFlecsNetworkIdComponent& NetworkId)
		{
			if UNLIKELY_IF(NetworkId.IsValid())
			{
				return;
			}
			
			NetworkId.SetNetworkId(GetNextNetworkId());

			UN_LOG(LogFlecsNetworkingManager, Log,
				"Assigned network ID %llu to entity %s",
				NetworkId.GetNetworkId(),
				*Entity.GetEntity().path().c_str());
			
		});

		NetworkPushModelObserver = FlecsWorld
			->CreateObserver<FFlecsReplicatedTrait, FFlecsPushModelTrait>(TEXT("NetworkingPushModelObserver"))
			.event(flecs::OnAdd)
			.event(flecs::OnRemove)
			.event(flecs::OnSet)
			.event(flecs::UnSet)
			.yield_existing(true)
			.each([this](const FFlecsEntityHandle& Entity,
				FFlecsReplicatedTrait& Replicated, FFlecsPushModelTrait& PushModel)
			{
			
			});
	}

	#endif // WITH_SERVER_CODE
}

void UFlecsNetworkingManager::AddNetworkingActorComponent(UFlecsNetworkingActorComponent* NetworkingActorComponent)
{
	checkf(IsValid(NetworkingActorComponent), TEXT("NetworkingActorComponent must be valid"));

	NetworkingActorComponents.Add(NetworkingActorComponent);
}

void UFlecsNetworkingManager::RemoveNetworkingActorComponent(UFlecsNetworkingActorComponent* NetworkingActorComponent)
{
	NetworkingActorComponents.Remove(NetworkingActorComponent);
}
