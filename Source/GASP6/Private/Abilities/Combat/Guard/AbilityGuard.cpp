// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/Guard/AbilityGuard.h"

#include "Tags/MyTagManager.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Guard/EffectGuard.h"

UAbilityGuard::UAbilityGuard()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    this->ActivationOwnedTags.AddTag(MyTags::PlayerState::combat);
}

void UAbilityGuard::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    if (!asc->HasMatchingGameplayTag(MyTags::PlayerState::guard) && asc->GetTagCount(MyTags::PlayerState::perfect_guard) < 2)
    {
        this->ApplyGameplayEffectToOwner(
            Handle,
            ActorInfo,
            ActivationInfo,
            (UEffectPerfectGuard *)UEffectPerfectGuard::StaticClass()->GetDefaultObject(),
            this->GetAbilityLevel());
        return;
    }
    asc->ApplyGameplayEffectSpecToSelf(
        FGameplayEffectSpec(
            (UEffectGuard *)UEffectGuard::StaticClass()->GetDefaultObject(),
            this->MakeEffectContext(Handle, ActorInfo)
        )
    );
    // this->ApplyGameplayEffectToOwner(
    //     Handle,
    //     ActorInfo,
    //     ActivationInfo,
    //     (UEffectGuard *)UEffectGuard::StaticClass()->GetDefaultObject(),
    //     this->GetAbilityLevel());
}

void UAbilityGuard::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    // UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    // asc->RemoveLooseGameplayTag(MyTags::PlayerState::guard);
}
