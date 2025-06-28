// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Stamina/EffectRecoverStamina.h"

#include "Attribute/Stamina/AttributeStamina.h"
#include "Tags/MyTagManager.h"

UEffectRecoverStamina::UEffectRecoverStamina()
{
    this->DurationPolicy = EGameplayEffectDurationType::Infinite;
    this->Period = 0.0166f;

    FGameplayModifierInfo mod;
    
    FSetByCallerFloat mag;
    mag.DataTag = MyTags::Effect::Stamina::recover;

    mod.Attribute = UAttributeStamina::GetStaminaAttribute();
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(mag);
    mod.ModifierOp = EGameplayModOp::Additive;
    this->Modifiers.Add(mod);

    UTargetTagRequirementsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagRequirementsGameplayEffectComponent>(TEXT("Hayya"));
    comp->OngoingTagRequirements.RequireTags.AddTag(MyTags::Attribute::Stamina::not_full);
    comp->OngoingTagRequirements.IgnoreTags.AddTag(MyTags::Effect::Stamina::cooldown);
    comp->OngoingTagRequirements.IgnoreTags.AddTag(MyTags::PlayerState::combat);
    
    this->GEComponents.Add(comp);
}