#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DDDAbilitySystemComponent.generated.h"

class UDDDAttributeSet;

UCLASS()
class DDD_API UDDDAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UDDDAbilitySystemComponent();

    UDDDAttributeSet* GetAttributeSet() const { return AttributeSet; }

private:
    UPROPERTY()
    TObjectPtr<UDDDAttributeSet> AttributeSet;
};
