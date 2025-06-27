// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityHandleGetHit.generated.h"

/**
 *
 */
UCLASS()
class UAbilityHandleGetHit : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityHandleGetHit();

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData);

private:
	class UAbilityGuard *MyAbilityGuard = nullptr;
	void FindMyAbilityGuard();
};
