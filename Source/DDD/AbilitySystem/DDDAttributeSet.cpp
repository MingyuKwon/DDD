#include "AbilitySystem/DDDAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UDDDAttributeSet::UDDDAttributeSet()
{
    InitHealth(100.f);
    InitMaxHealth(100.f);
}

void UDDDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UDDDAttributeSet, Health,    COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UDDDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
        SetMaxHealth(FMath::Max(GetMaxHealth(), 0.f));
        // MaxHealth 가 줄어들어 현재 Health 를 초과할 경우 클램핑
        SetHealth(FMath::Min(GetHealth(), GetMaxHealth()));
    }
}

void UDDDAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDDAttributeSet, Health, OldHealth);
}

void UDDDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDDAttributeSet, MaxHealth, OldMaxHealth);
}
