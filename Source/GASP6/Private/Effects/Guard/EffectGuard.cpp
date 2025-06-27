// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Guard/EffectGuard.h"

#include "Tags/MyTagManager.h"

UEffectGuard::UEffectGuard()
{
    this->StackingType = EGameplayEffectStackingType::AggregateByTarget;
    this->StackLimitCount = 1;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
    // this->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::RemoveSingleStackAndRefreshDuration;
    this->bDenyOverflowApplication = false;

    // TODO: Change to duration
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(5.0f);
    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(FName("Never cae"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::PlayerState::guard);
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::PlayerState::combat);
    comp->SetAndApplyTargetTagChanges(comp->GetConfiguredTargetTagChanges());

    this->GEComponents.Add(comp);
}
