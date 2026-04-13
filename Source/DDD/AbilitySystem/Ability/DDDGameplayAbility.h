#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDDGameplayAbility.generated.h"

/**
 * DDD 프로젝트 어빌리티 베이스 클래스.
 * 모든 어빌리티는 이 클래스를 상속받아 구현한다.
 */
UCLASS(Abstract)
class DDD_API UDDDGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UDDDGameplayAbility();

protected:
    /** 어빌리티 발동 */
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    /** 어빌리티 종료 (bWasCancelled: true 면 취소, false 면 정상 종료) */
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;
};
