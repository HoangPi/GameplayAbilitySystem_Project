// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySprint.generated.h"

/**
 * 
 */
UCLASS()
class UAbilitySprint : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilitySprint();

	void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData);

	void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);

private:
	FActiveGameplayEffectHandle effectDrainStamina;
};
