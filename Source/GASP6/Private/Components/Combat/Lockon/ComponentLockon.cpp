// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/Lockon/ComponentLockon.h"

#include "GASP6/GASP6Character.h"
#include "Abilities/Combat/Lockon/AbilityLockon.h"
#include "Abilities/Combat/Lockon/AbilityLockonSwitch.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UComponentLockon::UComponentLockon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->SetComponentTickEnabled(false);

	// ...
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		this->ownerASC = owner->GetAbilitySystemComponent();

		this->LockonAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon"));
		this->LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"));
		this->lockonAbilityHandle = this->ownerASC->K2_GiveAbility(UAbilityLockon::StaticClass());
		this->switchAbilityHandle = this->ownerASC->K2_GiveAbility(UAbilityLockonSwitch::StaticClass());
	}
}

// Called when the game starts
void UComponentLockon::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (this->LockonAction)
	{
		this->SetupMyInputs();
	}
}

// Called every frame
void UComponentLockon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!this->Target)
	{
		this->SetComponentTickEnabled(false);
		return;
	}
	// TODO: Type check this if erorr
	AGASP6Character *owner = ((AGASP6Character *)this->GetOwner());
	if (!owner->GetController()->LineOfSightTo(this->Target, owner->GetFollowCamera()->GetComponentLocation()))
	{
		this->SetComponentTickEnabled(false);
		this->Target = nullptr;
		return;
	}
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), this->Target->GetActorLocation());
	targetRot.Pitch -= 20.0;
	targetRot = UKismetMathLibrary::RInterpTo(
		owner->GetController()->GetControlRotation(),
		targetRot,
		DeltaTime,
		10.0f);
	owner->GetController()->SetControlRotation(targetRot);
	// TODO: maybe check the rotation angle to play anim
	if (!owner->GetCharacterMovement()->bOrientRotationToMovement)
	{
		owner->SetActorRotation(FRotator(0, targetRot.Yaw, 0));
	}
#ifdef WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.0f,
		FColor::Green,
		FString::Printf(
			TEXT("%fs remaining"), 
			((UAbilityLockonSwitch *)UAbilityLockonSwitch::StaticClass()->GetDefaultObject())->GetCooldownTimeRemaining(ownerASC->AbilityActorInfo.Get())
		)
	);
#endif
}

void UComponentLockon::SetupMyInputs()
{
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		UEnhancedInputComponent *ownerInput = Cast<UEnhancedInputComponent>(owner->InputComponent);
		if (ownerInput)
		{
			ownerInput->BindAction(this->LockonAction, ETriggerEvent::Started, this, &UComponentLockon::LockOn);
			ownerInput->BindAction(this->LookAction, ETriggerEvent::Triggered, this, &UComponentLockon::switchTarget);
		}
	}
}

void UComponentLockon::LockOn()
{
	if (this->Target)
	{
		this->LockOff();
		return;
	}
	this->FindAndSetTarget();
	if (this->Target)
	{
		// this->lookBinding = ownerInput->BindAction(this->LookAction, ETriggerEvent::Started, this, &UComponentLockon::switchTarget).GetHandle();
	}
}

void UComponentLockon::FindAndSetTarget()
{
	this->ownerASC->TryActivateAbility(this->lockonAbilityHandle);
}
void UComponentLockon::LockOff()
{
	this->ownerASC->CancelAbilityHandle(this->lockonAbilityHandle);
}
void UComponentLockon::switchTarget(const FInputActionValue &Value)
{
	if (!this->Target)
	{
		return;
	}
	FVector2d input = Value.Get<FVector2d>();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, FString::Printf(TEXT("%f"), input.X));
	if (input.X >= 5.0f)
	{
		this->SwitchLeft = false;
		this->ownerASC->TryActivateAbility(this->switchAbilityHandle);
		return;
	}
	if (input.X <= -5.0f)
	{
		this->SwitchLeft = true;
		this->ownerASC->TryActivateAbility(this->switchAbilityHandle);
		return;
	}
}