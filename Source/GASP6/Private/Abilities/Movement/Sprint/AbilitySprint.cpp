// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Movement/Sprint/AbilitySprint.h"

#include "Tags/MyTagManager.h"
#include "GASP6/GASP6Character.h"
#include "Effects/Stamina/EffectDrainStamina.h"
#include "Components/Combat/Lockon/ComponentLockon.h"

UAbilitySprint::UAbilitySprint()
{
    ((FGameplayTagContainer &)this->GetAssetTags()).AddTag(MyTags::Ability::Requirement::stamina);
    this->SetAssetTags(this->GetAssetTags());

    this->ActivationOwnedTags.AddTag(MyTags::PlayerState::combat);
    this->ActivationOwnedTags.AddTag(MyTags::Ability::Movement::sprint);
    this->ActivationBlockedTags.AddTag(MyTags::Attribute::Stamina::empty);
}

void UAbilitySprint::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    AGASP6Character *owner = Cast<AGASP6Character>(ActorInfo->AvatarActor.Get());
    if (owner)
    {
        UCharacterMovementComponent *movement = owner->GetCharacterMovement();
        movement->MaxWalkSpeed = 500.0f;
        movement->bOrientRotationToMovement = true;
        FGameplayEffectSpecHandle specHandle = this->GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
            UEffectDrainStamina::StaticClass(),
            this->GetAbilityLevel(),
            this->GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
        // TODO: Apply logic (mainly from ability level) to adjust the magnitude
        specHandle.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::Stamina::drain, -2.7f);

        this->effectDrainStamina = this->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
    }
}

void UAbilitySprint::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    AGASP6Character *owner = Cast<AGASP6Character>(ActorInfo->AvatarActor.Get());
    if (owner)
    {
        UCharacterMovementComponent *movement = owner->GetCharacterMovement();
        movement->MaxWalkSpeed = 230.0f;
        movement->bOrientRotationToMovement = (owner->myLockonComponent->Target ? false : true);
        this->GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(this->effectDrainStamina);
    }
}