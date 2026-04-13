#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DDDGameplayAbility.h"
#include "BulletShotAbility.generated.h"

/**
 * 총알을 발사하는 어빌리티.
 * ActivateAbility 에서 발사체 스폰 로직을 구현한다.
 */
UCLASS()
class DDD_API UBulletShotAbility : public UDDDGameplayAbility
{
    GENERATED_BODY()

public:
    UBulletShotAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayTagContainer* SourceTags = nullptr, 
        const FGameplayTagContainer* TargetTags = nullptr, 
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    virtual bool CheckCost(
        const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    virtual void ApplyCost(
        const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:
    /** 발사체 클래스 (에디터에서 지정) */
    UPROPERTY(EditDefaultsOnly, Category = "BulletShot")
    TSubclassOf<AActor> ProjectileClass;
};
