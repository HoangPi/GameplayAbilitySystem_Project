// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/Guard/AbilityGuard.h"

#include "Tags/MyTagManager.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Guard/EffectGuard.h"

UAbilityGuard::UAbilityGuard()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // this->ActivationOwnedTags.AddTag(MyTags::PlayerState::combat);
    this->ActivationOwnedTags.AddTag(MyTags::PlayerState::manual_guard);
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

void UAbilityGuard::HandleGetHit()
{
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    if (asc->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass(), nullptr) > 0)
    {
        asc->RemoveActiveGameplayEffect(this->PerfectGuardEffectHandle, 1);
        // TODO: run perfect guard instant effect
        return;
    }
    if (asc->HasMatchingGameplayTag(MyTags::PlayerState::guard))
    {
        asc->RemoveActiveGameplayEffect(this->GuardEffectHandle, -1);
        // TODO: run guard instant effect
        // asc->RemoveLooseGameplayTag(MyTags::PlayerState::guard);
        return;
    }
    if (asc->HasMatchingGameplayTag(MyTags::PlayerState::manual_guard))
    {
        // TODO: run guard instant effect
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Manually blocked"));
        return;
    }
    // TODO: Run get hit instant effect
}