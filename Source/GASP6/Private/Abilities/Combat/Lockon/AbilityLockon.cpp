// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Combat/Lockon/AbilityLockon.h"

#include "GASP6/GASP6Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Combat/Lockon/ComponentLockon.h"

UAbilityLockon::UAbilityLockon()
{
    this->objectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
    this->collisionShape = FCollisionShape::MakeBox(FVector(100, 400, 400));
}

void UAbilityLockon::ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    AGASP6Character *owner = Cast<AGASP6Character>(ActorInfo->AvatarActor.Get());
    if(owner)
    {
        this->actorsToIgnore.ClearIgnoredSourceObjects();
        this->actorsToIgnore.AddIgnoredActor(owner);
        FHitResult result;
        FVector start = owner->GetFollowCamera()->GetComponentLocation();
        FRotator rot = owner->GetFollowCamera()->GetComponentRotation();
        rot.Pitch += 15;
        rot.Normalize();
        FVector end = start + rot.Vector() * 3000;
        // TODO: Cache the sum (if must)
        start = start + rot.Vector() * (this->collisionShape.Box.HalfExtentX + owner->GetCameraBoom()->TargetArmLength);
        if(this->GetWorld()->SweepSingleByObjectType(
            result,
            start,
            end,
            FQuat(rot),
            this->objectTypes,
            this->collisionShape,
            this->actorsToIgnore
        ))
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, result.GetActor()->GetName());
            owner->myLockonComponent->Target = result.GetActor();
            owner->myLockonComponent->SetComponentTickEnabled(true);
        }
        // TArray<TEnumAsByte<EObjectTypeQuery>> ehm;
        // ehm.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
        // TArray<AActor *> yo;
        // yo.Add(owner);
        // UKismetSystemLibrary::BoxTraceSingleForObjects(
        //     this->GetWorld(),
        //     start,
        //     end,
        //     this->collisionShape.GetBox(),
        //     rot,
        //     ehm,
        //     false,
        //     yo,
        //     EDrawDebugTrace::ForDuration,
        //     result,
        //     false,
        //     FLinearColor::Red,
        //     FLinearColor::Green,
        //     2.0f
        // );
    }
}

void UAbilityLockon::EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    AGASP6Character *owner = Cast<AGASP6Character>(ActorInfo->AvatarActor.Get());
    if(owner)
    {
        owner->myLockonComponent->Target = nullptr;
    }
}