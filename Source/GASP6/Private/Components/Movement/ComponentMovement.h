// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

#include "ComponentMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UComponentMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UAbilitySystemComponent *ownerASC;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	void SetupMyInputs();
	void Sprint();
	void StopSprinting();

	FGameplayAbilitySpecHandle AbilitySprint;
	class UAttributeStamina *Stamina;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
