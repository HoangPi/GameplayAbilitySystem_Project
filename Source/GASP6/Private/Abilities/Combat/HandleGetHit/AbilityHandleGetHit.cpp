// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/HandleGetHit/AbilityHandleGetHit.h"

#include "Tags/MyTagManager.h"
#include "Abilities/Combat/Guard/AbilityGuard.h"
#include "Effects/Guard/EffectGuard.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Health/EffectHealthModifier.h"
#include "AbilityHandleGetHit.h"

UAbilityHandleGetHit::UAbilityHandleGetHit()
{
    FAbilityTriggerData triggerEvent;
    triggerEvent.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    triggerEvent.TriggerTag = MyTags::Ability::attacked;
    this->AbilityTriggers.Add(triggerEvent);
}

void UAbilityHandleGetHit::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    if (!TriggerEventData)
    {
        return;
    }
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    if (!this->MyAbilityGuard)
    {
        this->FindMyAbilityGuard();
    }
    if (asc->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass(), nullptr) > 0)
    {
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->PerfectGuardEffectHandle, 1);
        // TODO: run perfect guard instant effect
        return;
    }
    if (asc->HasMatchingGameplayTag(MyTags::PlayerState::guard))
    {
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->GuardEffectHandle, -1);
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
    FGameplayEffectSpecHandle effectSpecHandle = this->MakeOutgoingGameplayEffectSpec(
        UEffectHealthModifier::StaticClass(),
        this->GetAbilityLevel());
    effectSpecHandle.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::health, -TriggerEventData->EventMagnitude);
    this->ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, effectSpecHandle);
}

void UAbilityHandleGetHit::FindMyAbilityGuard()
{
    // TODO: This is super unsafe, consider some guards condition in the future
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    FGameplayAbilitySpec *GuardSpec = asc->FindAbilitySpecFromClass(UAbilityGuard::StaticClass());
    this->MyAbilityGuard = (UAbilityGuard *)GuardSpec->GetPrimaryInstance();
}