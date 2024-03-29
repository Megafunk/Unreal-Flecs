﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FlecsNetworkingActorComponent.h"
#include "Unlog/Unlog.h"
#include "Worlds/FlecsWorldSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlecsNetworkingActorComponent)

UNLOG_CATEGORY(LogFlecsNetworkingActorComponent);

void UFlecsNetworkingActorComponent::Client_UpdateCreatedNetworkedEntities_Implementation(
	const TArray<FNetworkedEntityInfo>& Entities)
{
	for (const auto& [NetworkId, WorldName, EntityName] : Entities)
	{
		checkf(!EntityName.IsNone(),TEXT("Networked Entity Name must not be None"));

		const UFlecsWorld* FlecsWorld = GetWorld()->GetSubsystem<UFlecsWorldSubsystem>()->GetFlecsWorld(WorldName);
		
		if UNLIKELY_IF(!ensureMsgf(FlecsWorld, TEXT("FlecsWorld %s not found"), *WorldName.ToString()))
		{
			continue;
		}
		
		FFlecsEntityHandle Entity = FlecsWorld->GetEntityWithName(EntityName.ToString());
		
		if UNLIKELY_IF(!Entity.IsValid())
		{
			UN_LOG(LogFlecsNetworkingActorComponent, Error,
				"Entity %s not found in world %s",
				*EntityName.ToString(),
				*WorldName.ToString());
			
			continue;
		}

		Entity.Set<FFlecsNetworkIdComponent>(NetworkId);

		UN_LOG(LogFlecsNetworkingActorComponent, Log,
			"Added network ID %llu to entity %s in world %s",
			NetworkId.GetNetworkId(),
			*EntityName.ToString(),
			*WorldName.ToString());
	}
}
