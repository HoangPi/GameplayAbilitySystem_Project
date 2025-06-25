// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/Guard/EffectPerfectGuard.h"

#include "Tags/MyTagManager.h"

UEffectPerfectGuard::UEffectPerfectGuard()
{
    this->StackingType = EGameplayEffectStackingType::AggregateBySource;
    this->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::NeverRefresh;
    this->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::RemoveSingleStackAndRefreshDuration;
    this->StackLimitCount = 2;

    this->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    // TODO: Change to duration
    this->DurationMagnitude = FGameplayEffectModifierMagnitude(2.0f);

    UTargetTagsGameplayEffectComponent *comp = this->CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(FName("Never care about this before"));
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::PlayerState::perfect_guard);
    ((FInheritedTagContainer &)comp->GetConfiguredTargetTagChanges()).AddTag(MyTags::PlayerState::combat);
    comp->SetAndApplyTargetTagChanges(comp->GetConfiguredTargetTagChanges());

    UTargetTagRequirementsGameplayEffectComponent *comp2 = this->CreateDefaultSubobject<UTargetTagRequirementsGameplayEffectComponent>(FName("Never care about this before1"));
    comp2->RemovalTagRequirements.RequireTags.AddTag(MyTags::PlayerState::guard);
    // this->ApplicationTagRequirements.IgnoreTags.AddTag(MyTags::PlayerState::guard);

    this->GEComponents.Add(comp2);
    this->GEComponents.Add(comp);
}