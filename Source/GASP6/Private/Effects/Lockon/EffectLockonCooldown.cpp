// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Lockon/EffectLockonCooldown.h"

#include "Tags/Ability/Lockon/TagLockon.h"

UEffectLockonCooldown::UEffectLockonCooldown()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(1.0f);
    this->StackingType = EGameplayEffectStackingType::None;

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>("GHGOVN");
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::Ability::Lockon::cooldown);
    comp->SetAndApplyTargetTagChanges(comp->GetConfiguredTargetTagChanges());
}