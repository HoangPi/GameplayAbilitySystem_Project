// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "AbilitySystemComponent.h"

#include "AbilityGuard.generated.h"

/**
 *
 */
UCLASS()
class UAbilityGuard : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityGuard();

	void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);

	void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);

	UPROPERTY(BlueprintReadOnly)
	FActiveGameplayEffectHandle PerfectGuardEffectHandle;

	FActiveGameplayEffectHandle GuardEffectHandle;
	
	UFUNCTION(BlueprintCallable)
	void HandleGetHit();
};
