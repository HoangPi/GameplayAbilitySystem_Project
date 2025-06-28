// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeHealth.generated.h"

/**
 * 
 */
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

private:
	inline virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) const;
};
