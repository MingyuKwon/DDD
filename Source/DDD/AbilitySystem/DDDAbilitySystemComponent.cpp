#include "AbilitySystem/DDDAbilitySystemComponent.h"

#include "AbilitySystem/DDDAttributeSet.h"

UDDDAbilitySystemComponent::UDDDAbilitySystemComponent()
{
    AttributeSet = CreateDefaultSubobject<UDDDAttributeSet>(TEXT("AttributeSet"));
}
