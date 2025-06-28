// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/Guard/AbilityGuard.h"

#include "Tags/MyTagManager.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Guard/EffectGuard.h"

UAbilityGuard::UAbilityGuard()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // this->ActivationOwnedTags.AddTag(MyTags::PlayerState::combat);
    this->ActivationBlockedTags.AddTag(MyTags::PlayerState::down);
    this->ActivationOwnedTags.AddTag(MyTags::PlayerState::manual_guard);

    ((FGameplayTagContainer &)this->GetAssetTags()).AddTag(MyTags::Ability::Requirement::stamina);
    this->SetAssetTags(this->GetAssetTags());
}

void UAbilityGuard::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    if (
        !asc->HasMatchingGameplayTag(MyTags::PlayerState::guard) &&
        asc->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass(), nullptr) < 2)
    {
        this->PerfectGuardEffectHandle = this->ApplyGameplayEffectToOwner(
            Handle,
            ActorInfo,
            ActivationInfo,
            (UEffectPerfectGuard *)UEffectPerfectGuard::StaticClass()->GetDefaultObject(),
            this->GetAbilityLevel());
        return;
    }
    this->GuardEffectHandle = this->ApplyGameplayEffectToOwner(
        Handle,
        ActorInfo,
        ActivationInfo,
        (UEffectGuard *)UEffectGuard::StaticClass()->GetDefaultObject(),
        this->GetAbilityLevel());
}

void UAbilityGuard::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}