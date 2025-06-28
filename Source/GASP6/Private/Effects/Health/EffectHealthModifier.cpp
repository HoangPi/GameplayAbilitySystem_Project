// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Health/EffectHealthModifier.h"

#include "Attribute/Health/AttributeHealth.h"
#include "Tags/Effect/TagEffect.h"

UEffectHealthModifier::UEffectHealthModifier()
{
    this->StackingType = EGameplayEffectStackingType::None;
    this->DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo mod;
    mod.Attribute = UAttributeHealth::GetHealthAttribute();
    mod.ModifierOp = EGameplayModOp::Additive;

    FSetByCallerFloat setBycaller;
    setBycaller.DataTag = MyTags::Effect::health;
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(setBycaller);
    this->Modifiers.Add(mod);
}