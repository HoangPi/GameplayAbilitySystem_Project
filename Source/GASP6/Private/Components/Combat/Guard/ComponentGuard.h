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

public:
	// UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent *ownerASC;
	// UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle GuardAbilitySpecHandle;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	const FGameplayAbilitySpecHandle GetGuardAbilitySpecHandle() { return this->GuardAbilitySpecHandle; };

private:
	void SetupMyInputs();
	void Guard();
	void ReleaseGuard();
};
