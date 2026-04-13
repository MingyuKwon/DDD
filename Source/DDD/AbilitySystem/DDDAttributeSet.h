#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DDDAttributeSet.generated.h"

// Health / MaxHealth 게터·세터·이니셜라이저를 한 번에 선언하는 헬퍼 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DDD_API UDDDAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UDDDAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** GameplayEffect 적용 후 후처리 (클램핑 등) */
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

    // ── Health ───────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UDDDAttributeSet, Health)

    // ── MaxHealth ─────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UDDDAttributeSet, MaxHealth)

private:
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
