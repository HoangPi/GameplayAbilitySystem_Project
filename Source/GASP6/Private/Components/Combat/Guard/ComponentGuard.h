// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"

#include "ComponentGuard.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UComponentGuard : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponentGuard();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UInputAction *GuardAction;
	UAbilitySystemComponent *ownerASC;
	FGameplayAbilitySpecHandle GuardAbilitySpecHandle;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	void SetupMyInputs();
	void Guard();
	void ReleaseGuard();
};
