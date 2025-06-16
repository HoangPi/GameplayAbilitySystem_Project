// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"

#include "AttributeStamina.generated.h"

/**
 *
 */
UCLASS()
class UAttributeStamina : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAttributeStamina();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Stamina;

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, Stamina)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxStamina;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeStamina, MaxStamina)

private:
	FGameplayTagContainer abilitiesToCancel;
	bool hasFull = true;
	bool hasEmpty = false;
	bool hasNotFull = false;

public:
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data);
};
