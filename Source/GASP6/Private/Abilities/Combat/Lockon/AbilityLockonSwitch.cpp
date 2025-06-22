// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Combat/Lockon/AbilityLockonSwitch.h"

#include "Effects/Lockon/EffectLockonCooldown.h"
#include "GASP6/GASP6Character.h"
#include "Components/Combat/Lockon/ComponentLockon.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityLockonSwitch::UAbilityLockonSwitch()
{
	this->CooldownGameplayEffectClass = UEffectLockonCooldown::StaticClass();
	this->objectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	this->collisionShape = FCollisionShape::MakeBox(FVector(100, 400, 400));
}

void UAbilityLockonSwitch::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AGASP6Character *owner = Cast<AGASP6Character>(ActorInfo->AvatarActor.Get());
	if (owner)
	{
		this->actorsToIgnore.ClearIgnoredSourceObjects();
		this->actorsToIgnore.AddIgnoredActor(owner);
		this->actorsToIgnore.AddIgnoredActor(owner->myLockonComponent->Target);

		TArray<FHitResult> results;
		FVector start = owner->GetFollowCamera()->GetComponentLocation();
		FRotator rot = owner->GetFollowCamera()->GetComponentRotation();
		FVector direction = rot.RotateVector(FVector(0, (owner->myLockonComponent->SwitchLeft ? -1 : 1), 0));
		rot.Pitch += 20;
		start += direction * this->collisionShape.Box.HalfExtentY;
		start += rot.Vector() * (this->collisionShape.Box.HalfExtentX + owner->GetCameraBoom()->TargetArmLength);
		FVector end = start + rot.Vector() * 3000;
		if (this->GetWorld()->SweepMultiByObjectType(
				results,
				start,
				end,
				FQuat(rot),
				this->objectTypes,
				this->collisionShape,
				this->actorsToIgnore))
		{
			this->ApplyCooldown(Handle, ActorInfo, ActivationInfo);
			AActor *target = results[0].GetActor();
			FVector base = owner->myLockonComponent->Target->GetActorLocation();
			double currentDistance = FVector::Distance(
				target->GetActorLocation(),
				base);
			double newDistance = 0;
			// TODO: Update the limit if neccessary
			// Only evaluate the first 4 hits
			for (int i = 1; i < (results.Num() > 4 ? 4 : results.Num()); i++)
			{
				newDistance = FVector::Distance(
					results[i].GetActor()->GetActorLocation(),
					base);
				if (newDistance < currentDistance)
				{
					currentDistance = newDistance;
					target = results[i].GetActor();
				}
			}
			owner->myLockonComponent->Target = target;
		}
#ifdef WITH_EDITOR
		TArray<TEnumAsByte<EObjectTypeQuery>> ehm;
		ehm.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		TArray<AActor *> yo;
		yo.Add(owner);
		FHitResult r;
		UKismetSystemLibrary::BoxTraceSingleForObjects(
			this->GetWorld(),
			start,
			end,
			this->collisionShape.GetBox(),
			rot,
			ehm,
			false,
			yo,
			EDrawDebugTrace::ForDuration,
			r,
			false,
			FLinearColor::Red,
			FLinearColor::Green,
			2.0f);
	}
#endif
}

void UAbilityLockonSwitch::EndAbility(const FGameplayAbilitySpecHandle Handle,
									  const FGameplayAbilityActorInfo *ActorInfo,
									  const FGameplayAbilityActivationInfo ActivationInfo,
									  bool bReplicateEndAbility,
									  bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}