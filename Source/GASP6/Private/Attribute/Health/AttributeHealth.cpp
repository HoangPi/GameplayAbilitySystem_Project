// Fill out your copyright notice in the Description page of Project Settings.

#include "Attribute/Health/AttributeHealth.h"

#include "Ultilities/MyMacros.h"

UAttributeHealth::UAttributeHealth()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, TEXT("/Game/ThirdPerson/DataTable/DT_Health.DT_Health"));
    if (table)
    {
        // TODO: Properly set the row name
        FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("3"), FString("StillDontKnowWhatThisDoes"));
        this->MaxHealth = data->BaseValue;
        this->Health = data->BaseValue;
    }
}

void UAttributeHealth::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
    float max;
    if(UAttributeHealth::GetHealthAttribute() == Attribute)
    {
        max = this->MaxHealth.GetCurrentValue();
        NewValue = CLAMP(NewValue, max, 0.0f);
    }
}

void UAttributeHealth::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    if(Data.EvaluatedData.Attribute == UAttributeHealth::GetHealthAttribute())
    {
        this->OnHealthChangeEvent.Broadcast(this->Health.GetCurrentValue()/this->MaxHealth.GetCurrentValue());
    }
}