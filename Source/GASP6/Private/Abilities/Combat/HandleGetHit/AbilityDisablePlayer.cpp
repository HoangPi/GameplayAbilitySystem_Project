// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/HandleGetHit/AbilityDisablePlayer.h"

#include "Effects/PlayerStates/EffectPlayerDown.h"
#include "Tags/PlayerState/TagPlayerState.h"

UAbilityDisablePlayer::UAbilityDisablePlayer()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // this->ActivationBlockedTags.AddTag(MyTags::PlayerState::down);
}

void UAbilityDisablePlayer::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_WaitGameplayEffectRemoved *task = UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(
        this,
        this->ApplyGameplayEffectToOwner(
            Handle,
            ActorInfo,
            ActivationInfo,
            (UEffectPlayerDown *)UEffectPlayerDown::StaticClass()->GetDefaultObject(),
            1.0f));
    task->OnRemoved.AddDynamic(this, &UAbilityDisablePlayer::NotifyPlayerRecoverHandler);
    task->ReadyForActivation();
    this->NotifyPlayerDown.Execute();
}

void UAbilityDisablePlayer::NotifyPlayerRecoverHandler(const FGameplayEffectRemovalInfo & unused)
{
    this->NotifyPlayerRecover.Execute();
    Super::EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}