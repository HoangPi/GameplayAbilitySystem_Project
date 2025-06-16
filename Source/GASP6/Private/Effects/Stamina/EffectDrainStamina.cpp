// Fill out your copyright notice in the Description page of Project Settings.

#include "Effects/Stamina/EffectDrainStamina.h"

#include "Tags/MyTagManager.h"
#include "Attribute/Stamina/AttributeStamina.h"

UEffectDrainStamina::UEffectDrainStamina()
{
    this->DurationPolicy = EGameplayEffectDurationType::Infinite;
    this->Period = 0.1f;
    this->StackingType = EGameplayEffectStackingType::None;
    ((FGameplayTagContainer &)this->GetAssetTags()).AddTag(MyTags::Effect::Stamina::drain);

    FGameplayModifierInfo mod;
    mod.Attribute = UAttributeStamina::GetStaminaAttribute();
    // mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(-5.1f);

    FSetByCallerFloat mag;
    mag.DataTag = MyTags::Effect::Stamina::drain;
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(mag);
    mod.ModifierOp = EGameplayModOp::Additive;

    this->Modifiers.Add(mod);
}