// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"

#include "AbilityDisablePlayer.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnNotifyPlayerDown, bool);

UCLASS()
class UAbilityDisablePlayer : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityDisablePlayer();

	FOnNotifyPlayerDown NotifyPlayerDown;

	void ActivateAbility(
		FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo *ActorInfo, 
		FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData *TriggerEventData);

	UFUNCTION()
	void NotifyPlayerRecoverHandler(const FGameplayEffectRemovalInfo & unused);
};
