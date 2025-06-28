// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/HandleGetHit/AbilityHandleGetHit.h"

#include "Tags/MyTagManager.h"
#include "Abilities/Combat/Guard/AbilityGuard.h"
#include "Effects/Guard/EffectGuard.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Health/EffectHealthModifier.h"
#include "Effects/Stamina/EffectStaminaInstant.h"
#include "AbilityHandleGetHit.h"

UAbilityHandleGetHit::UAbilityHandleGetHit()
{
    FAbilityTriggerData triggerEvent;
    triggerEvent.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    triggerEvent.TriggerTag = MyTags::Ability::attacked;
    this->AbilityTriggers.Add(triggerEvent);
}

// TODO: promote those logics into a function
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
        // Remove a perfect guard stack
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->PerfectGuardEffectHandle, 1);
        // Reduce stamina
        FGameplayEffectSpecHandle effectSpecHandle = this->MakeOutgoingGameplayEffectSpec(
            UEffectStaminaInstant::StaticClass(),
            this->GetAbilityLevel());
        effectSpecHandle.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::stamina, -TriggerEventData->EventMagnitude/2);
        this->ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, effectSpecHandle);
        return;
    }
    if (asc->HasMatchingGameplayTag(MyTags::PlayerState::guard))
    {
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->GuardEffectHandle, -1);
        FGameplayEffectSpecHandle effectSpecHandle = this->MakeOutgoingGameplayEffectSpec(
            UEffectStaminaInstant::StaticClass(),
            this->GetAbilityLevel());
        effectSpecHandle.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::stamina, -TriggerEventData->EventMagnitude);
        this->ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, effectSpecHandle);
        return;
    }
    if (asc->HasMatchingGameplayTag(MyTags::PlayerState::manual_guard))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Manually blocked"));
        FGameplayEffectSpecHandle effectSpecHandle = this->MakeOutgoingGameplayEffectSpec(
            UEffectStaminaInstant::StaticClass(),
            this->GetAbilityLevel());
        effectSpecHandle.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::stamina, -TriggerEventData->EventMagnitude);
        this->ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, effectSpecHandle);
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