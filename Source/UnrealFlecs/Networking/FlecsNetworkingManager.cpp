﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FlecsNetworkingManager.h"
#include "FlecsEntitySyncInfoComponent.h"
#include "FlecsNetworkIdComponent.h"
#include "FlecsNetworkingActorComponent.h"
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

	/* Networked Entities require a Stable Name to begin with,
	 this name can be changed after the entity's network ID is assigned */

	if (GetOwner()->HasAuthority())
	{
		/*NetworkIdObserver =
			FlecsWorld->CreateObserver<FFlecsNetworkIdComponent>(TEXT("NetworkingIdObserver"))
		.term_at(1)
			.event(flecs::OnAdd)
			.yield_existing(true)
			.read_write()
		.with<FFlecsEntitySyncInfoComponent>()
			.optional()
		.with(flecs::Name)
			.and_()
			.inout_none()
		.each([this](flecs::iter& Iterator, const uint64 Index, FFlecsNetworkIdComponent& NetworkId)
		{
			FFlecsEntityHandle Entity = Iterator.entity(Index);

			FFlecsEntitySyncInfoComponent* EntitySyncInfo = Entity.GetPtr<FFlecsEntitySyncInfoComponent>();
			
			if UNLIKELY_IF(NetworkId.IsValid())
			{
				return;
			}
			
			NetworkId.SetNetworkId(GetNextNetworkId());

			UN_LOG(LogFlecsNetworkingManager, Log,
				"Assigned network ID %llu to entity %s",
				NetworkId.GetNetworkId(),
				*Entity.GetEntity().path().c_str());

			
		});*/
	}

	#endif // WITH_SERVER_CODE
}

void UFlecsNetworkingManager::AddNetworkingActorComponent(UFlecsNetworkingActorComponent* NetworkingActorComponent)
{
	solid_checkf(IsValid(NetworkingActorComponent), TEXT("NetworkingActorComponent must be valid"));

	NetworkingActorComponents.Add(NetworkingActorComponent);
}

void UFlecsNetworkingManager::RemoveNetworkingActorComponent(UFlecsNetworkingActorComponent* NetworkingActorComponent)
{
	NetworkingActorComponents.Remove(NetworkingActorComponent);
}
