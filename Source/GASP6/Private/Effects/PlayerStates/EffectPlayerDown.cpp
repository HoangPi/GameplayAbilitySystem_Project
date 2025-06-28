// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/PlayerStates/EffectPlayerDown.h"

#include "Tags/PlayerState/TagPlayerState.h"

UEffectPlayerDown::UEffectPlayerDown()
{
    this->StackingType = EGameplayEffectStackingType::None;
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    // TODO: Change the duration
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(5.0f);

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(FName("DisablePlayer"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::PlayerState::down);
    comp->SetAndApplyTargetTagChanges(comp->GetConfiguredTargetTagChanges());

    this->GEComponents.Add(comp);
}