// Fill out your copyright notice in the Description page of Project Settings.

#include "Attribute/Stamina/AttributeStamina.h"

#include "Tags/MyTagManager.h"
#include "Ultilities/MyMacros.h"
#include "GASP6/GASP6Character.h"

UAttributeStamina::UAttributeStamina()
{
    UDataTable *table = LoadObject<UDataTable>(nullptr, TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina"));
    if (table)
    {
        FAttributeMetaData *data = table->FindRow<FAttributeMetaData>(FName("2"), FString("insignificant"));
        this->Stamina.SetBaseValue(data->BaseValue);
        this->Stamina.SetCurrentValue(data->BaseValue);
        this->MaxStamina.SetBaseValue(data->BaseValue);
        this->MaxStamina.SetCurrentValue(data->BaseValue);

        this->abilitiesToCancel.AddTag(MyTags::Ability::Requirement::stamina);
    }
}

void UAttributeStamina::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
    if (Attribute == UAttributeStamina::GetStaminaAttribute())
    {
        float max = this->MaxStamina.GetCurrentValue();
        NewValue = CLAMP(NewValue, max, 0.0f);
    }
}

void UAttributeStamina::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    Super::PostGameplayEffectExecute(Data);
    float current;
    float max;
    AGASP6Character *owner = Cast<AGASP6Character>(Data.Target.GetOwnerActor());
    if (owner)
    {
    }
    if (Data.EvaluatedData.Attribute == UAttributeStamina::GetStaminaAttribute())
    {
        current = this->Stamina.GetCurrentValue();
        max = this->MaxStamina.GetCurrentValue();
        this->OnStaminaChange.Broadcast(current/max);
        if (current <= 0)
        {
            Data.Target.CancelAbilities(&this->abilitiesToCancel);
            Data.Target.AddLooseGameplayTag(MyTags::Attribute::Stamina::empty);
        }
        else if (current >= max)
        {
            Data.Target.RemoveLooseGameplayTag(MyTags::Attribute::Stamina::not_full);
            Data.Target.AddLooseGameplayTag(MyTags::Attribute::Stamina::full);
        }
        else
        {
            if (!Data.Target.HasMatchingGameplayTag(MyTags::Attribute::Stamina::not_full))
            {
                Data.Target.AddLooseGameplayTag(MyTags::Attribute::Stamina::not_full);
                Data.Target.RemoveLooseGameplayTag(MyTags::Attribute::Stamina::full);
            }
            Data.Target.RemoveLooseGameplayTag(MyTags::Attribute::Stamina::empty);
        }
    }
}