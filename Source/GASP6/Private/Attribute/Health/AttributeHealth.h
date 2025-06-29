// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

#include "AttributeHealth.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, percentage);

UCLASS()
class UAttributeHealth : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeHealth();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeHealth, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttributeHealth, MaxHealth)

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChangeEvent;

private:
	inline virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue);
	inline virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data);
};
