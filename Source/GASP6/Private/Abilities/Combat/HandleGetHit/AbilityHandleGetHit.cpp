// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/HandleGetHit/AbilityHandleGetHit.h"

#include "Tags/MyTagManager.h"
#include "Abilities/Combat/Guard/AbilityGuard.h"
#include "Effects/Guard/EffectGuard.h"
#include "Effects/Guard/EffectPerfectGuard.h"
#include "Effects/Health/EffectHealthModifier.h"
#include "Effects/Stamina/EffectStaminaInstant.h"
#include "Attribute/Stamina/AttributeStamina.h"
#include "AbilityHandleGetHit.h"
#include "Effects/PlayerStates/EffectPlayerDown.h"

UAbilityHandleGetHit::UAbilityHandleGetHit()
{
    this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    FAbilityTriggerData triggerEvent;
    triggerEvent.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    triggerEvent.TriggerTag = MyTags::Ability::attacked;
    this->AbilityTriggers.Add(triggerEvent);

    this->GuardContainer.AddTag(MyTags::PlayerState::guard);
    this->GuardContainer.AddTag(MyTags::PlayerState::manual_guard);

    this->ActivationOwnedTags.AddTag(MyTags::PlayerState::combat);
}

void UAbilityHandleGetHit::ActivateAbility(
    FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    // If not triggered via event, incorrect
    if (!TriggerEventData)
    {
        return;
    }
    if (!this->MyAbilityGuard)
    {
        this->FindMyAbilityGuard();
    }
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    FGameplayEffectSpecHandle effectSpecHandle;
    bool perfectGuard = false;
    // If has PERFECT GUARD tag, only reduce stamina by half of the magnitude
    // Also remove 1 stack of PERFECT GUARD
    if (asc->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass(), nullptr) > 0)
    {
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->PerfectGuardEffectHandle, 1);
        effectSpecHandle = this->CreateEffectSpecHandle(
            UEffectStaminaInstant::StaticClass(),
            MyTags::Effect::stamina,
            -TriggerEventData->EventMagnitude / 2);
        perfectGuard = true;
    }
    // If has GUARD or MANUAL GUARD tag, reduce stamina by the magnitude
    // Also remove all GUARD stack
    else if (asc->HasAnyMatchingGameplayTags(this->GuardContainer))
    {
        asc->RemoveActiveGameplayEffect(this->MyAbilityGuard->GuardEffectHandle, -1);
        effectSpecHandle = this->CreateEffectSpecHandle(
            UEffectStaminaInstant::StaticClass(),
            MyTags::Effect::stamina,
            -TriggerEventData->EventMagnitude);
    }
    // If not GUARD at all, reduce health
    else
    {
        effectSpecHandle = this->CreateEffectSpecHandle(
            UEffectHealthModifier::StaticClass(),
            MyTags::Effect::health,
            -TriggerEventData->EventMagnitude);
    }
    // Either way, apply the effect to owner
    this->ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, effectSpecHandle);
    // If stamina reach 0 but player perfectly guarded, player still stands
    if (!perfectGuard && asc->HasMatchingGameplayTag(MyTags::Attribute::Stamina::empty))
    {
        this->ApplyGameplayEffectToOwner(
            Handle,
            ActorInfo,
            ActivationInfo,
            (UEffectPlayerDown *)UEffectPlayerDown::StaticClass()->GetDefaultObject(),
            1.0f);
        this->NotifyPlayerDown.Execute();
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UAbilityHandleGetHit::FindMyAbilityGuard()
{
    // TODO: This is super unsafe, consider some guards condition in the future
    UAbilitySystemComponent *asc = this->GetAbilitySystemComponentFromActorInfo();
    FGameplayAbilitySpec *GuardSpec = asc->FindAbilitySpecFromClass(UAbilityGuard::StaticClass());
    this->MyAbilityGuard = (UAbilityGuard *)GuardSpec->GetPrimaryInstance();
}

FGameplayEffectSpecHandle UAbilityHandleGetHit::CreateEffectSpecHandle(
    TSubclassOf<UGameplayEffect> GameplayEffectClass,
    FGameplayTag DataTag,
    float Magnitude,
    float Level)
{
    FGameplayEffectSpecHandle effectSpecHandle = this->MakeOutgoingGameplayEffectSpec(GameplayEffectClass, Level);
    effectSpecHandle.Data.Get()->SetSetByCallerMagnitude(DataTag, Magnitude);
    return effectSpecHandle;
}