// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Movement/ComponentMovement.h"

#include "GASP6/GASP6Character.h"
#include "Attribute/Stamina/AttributeStamina.h"
#include "Abilities/Movement/Sprint/AbilitySprint.h"
#include "Tags/MyTagManager.h"
#include "Effects/Stamina/EffectLockStamina.h"
#include "Effects/Stamina/EffectRecoverStamina.h"

float yo = 0.0f;
// Sets default values for this component's properties
UComponentMovement::UComponentMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		this->ownerASC = owner->GetAbilitySystemComponent();
		this->SprintAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint"));

		this->AbilitySprint = this->ownerASC->K2_GiveAbility(UAbilitySprint::StaticClass(), 1);

		this->Stamina = this->CreateDefaultSubobject<UAttributeStamina>(TEXT("stamina_attribute"));
		this->ownerASC->AddAttributeSetSubobject(this->Stamina);

		// TODO: Cache the return value of AddLambda to cancel it when neccessary
		this->ownerASC->RegisterGameplayTagEvent(MyTags::PlayerState::combat).AddLambda([this](const FGameplayTag tag, int32 count)
																						{
				if(count <= 0)
				{
					// TODO: maybe cache the effect
					this->ownerASC->ApplyGameplayEffectToSelf(
						(UEffectLockStamina *)UEffectLockStamina::StaticClass()->GetDefaultObject(),
						1.0f,
						this->ownerASC->MakeEffectContext()
					);
					yo = 2.0f;
				} });
		this->ownerASC->AddLooseGameplayTag(MyTags::Attribute::Stamina::full);
	}
}

// Called when the game starts
void UComponentMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->SetupMyInputs();
	// TODO: Apply logic to adjust the level and magnitude
	FGameplayEffectSpecHandle effectSpec = this->ownerASC->MakeOutgoingSpec(
		UEffectRecoverStamina::StaticClass(),
		1.0f,
		this->ownerASC->MakeEffectContext());
	effectSpec.Data.Get()->SetSetByCallerMagnitude(MyTags::Effect::Stamina::recover, 2.1f);

	// TODO: Cache the effect to cancel it when neccessary
	this->ownerASC->ApplyGameplayEffectSpecToSelf(*effectSpec.Data.Get());
}

// Called every frame
void UComponentMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.0f,
		FColor::Green,
		FString::Printf(TEXT("%f/%f"), this->Stamina->Stamina.GetCurrentValue(), this->Stamina->MaxStamina.GetCurrentValue()));
	for (auto &&tag : this->ownerASC->GetOwnedGameplayTags())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, tag.GetTagName().ToString());
	}
	if (yo >= 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("%f"), yo));
		yo -= DeltaTime;
	}
}

void UComponentMovement::SetupMyInputs()
{
	AGASP6Character *owner = Cast<AGASP6Character>(this->GetOwner());
	if (owner)
	{
		UEnhancedInputComponent *input = Cast<UEnhancedInputComponent>(owner->InputComponent);
		if (input)
		{
			input->BindAction(this->SprintAction, ETriggerEvent::Started, this, &UComponentMovement::Sprint);
			input->BindAction(this->SprintAction, ETriggerEvent::Completed, this, &UComponentMovement::StopSprinting);
		}
	}
}
void UComponentMovement::Sprint()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Yes"));
	this->ownerASC->TryActivateAbility(this->AbilitySprint);
}
void UComponentMovement::StopSprinting()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("No"));
	this->ownerASC->CancelAbilityHandle(this->AbilitySprint);
}