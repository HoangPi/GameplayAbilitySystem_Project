// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "AbilityLockonSwitch.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityLockonSwitch : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityLockonSwitch();

	void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData);

	void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);

private:
	FCollisionObjectQueryParams objectTypes;
	FCollisionShape collisionShape;
	FCollisionQueryParams actorsToIgnore;
};
