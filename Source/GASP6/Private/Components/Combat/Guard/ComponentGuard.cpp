// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/Guard/ComponentGuard.h"

#include "GASP6/GASP6Character.h"
#include "Abilities/Combat/Guard/AbilityGuard.h"
#include "Effects/Guard/EffectPerfectGuard.h"

// Sets default values for this component's properties
UComponentGuard::UComponentGuard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	AGASP6Character *owner = this->GetOwner<AGASP6Character>();
	if (owner)
	{
		this->GuardAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Block.IA_Block"));
		this->ownerASC = owner->GetAbilitySystemComponent();

		this->GuardAbilitySpecHandle = this->ownerASC->K2_GiveAbility(UAbilityGuard::StaticClass());
	}
}

// Called when the game starts
void UComponentGuard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->SetupMyInputs();
}

// Called every frame
void UComponentGuard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.0f,
		FColor::Green,
		FString::Printf(
			TEXT("%d stacks"),
			ownerASC->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass(), nullptr))
		// FString::Printf(TEXT("%d stacks"), ownerASC->GetGameplayEffectCount(UEffectPerfectGuard::StaticClass()))
	);
}

void UComponentGuard::SetupMyInputs()
{
	AGASP6Character *owner = this->GetOwner<AGASP6Character>();
	if (owner)
	{
		UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent);
		if (input)
		{
			if (this->GuardAction)
			{
				input->BindAction(this->GuardAction, ETriggerEvent::Started, this, &UComponentGuard::Guard);
				input->BindAction(this->GuardAction, ETriggerEvent::Completed, this, &UComponentGuard::ReleaseGuard);
			}
		}
	}
}

void UComponentGuard::Guard()
{
	this->ownerASC->TryActivateAbility(this->GuardAbilitySpecHandle);
}

void UComponentGuard::ReleaseGuard()
{
	this->ownerASC->CancelAbilityHandle(this->GuardAbilitySpecHandle);
}