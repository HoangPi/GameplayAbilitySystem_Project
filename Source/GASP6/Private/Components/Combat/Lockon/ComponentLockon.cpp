// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/Lockon/ComponentLockon.h"

#include "GASP6/GASP6Character.h"

// Sets default values for this component's properties
UComponentLockon::UComponentLockon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		this->ownerASC = owner->GetAbilitySystemComponent();

		this->LockonAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon"));
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
}

void UComponentLockon::SetupMyInputs()
{
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent);
		if (input)
		{
			input->BindAction(this->LockonAction, ETriggerEvent::Started, this, &UComponentLockon::LockOn);
		}
	}
}

void UComponentLockon::LockOn()
{
	
}

void UComponentLockon::FindAndSetTarget()
{

}
void UComponentLockon::LockOff()
{

}