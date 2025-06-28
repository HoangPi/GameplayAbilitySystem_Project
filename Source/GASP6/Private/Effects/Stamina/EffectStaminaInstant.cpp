// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Stamina/EffectStaminaInstant.h"

#include "Attribute/Stamina/AttributeStamina.h"
#include "Tags/MyTagManager.h"

UEffectStaminaInstant::UEffectStaminaInstant()
{
    this->StackingType = EGameplayEffectStackingType::None;
    this->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo mod;
    FSetByCallerFloat setByCaller;

    setByCaller.DataTag = MyTags::Effect::stamina;
    mod.Attribute = UAttributeStamina::GetStaminaAttribute();
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(setByCaller);

    this->Modifiers.Add(mod);
}