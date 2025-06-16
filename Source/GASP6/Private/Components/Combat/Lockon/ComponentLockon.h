// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "ComponentLockon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UComponentLockon : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentLockon();

	AActor *Target;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UAbilitySystemComponent *ownerASC;
	UInputAction* LockonAction;
	FGameplayAbilitySpecHandle lockonAbilityHandle;
	FGameplayAbilitySpecHandle switchAbilityHandle;
	UInputAction* LookAction;

private:
	void SetupMyInputs();
	void LockOn();
	void FindAndSetTarget();
	void LockOff();
	void switchTarget(const FInputActionValue &Value);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
