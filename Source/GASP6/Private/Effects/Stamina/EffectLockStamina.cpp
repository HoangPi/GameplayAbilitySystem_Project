// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Stamina/EffectLockStamina.h"

#include "Tags/Effect/Stamina/TagEffectStamina.h"

UEffectLockStamina::UEffectLockStamina()
{
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);
    this->StackingType = EGameplayEffectStackingType::None;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("hello_there"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::Effect::Stamina::cooldown);
    comp->SetAndApplyTargetTagChanges(comp->GetConfiguredTargetTagChanges());
}