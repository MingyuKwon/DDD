// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DDDPlayerState.generated.h"

class UDDDInventoryComponent;
/**
 * DDDPlayerState
 *
 * 기본 APlayerState의 NetUpdateFrequency(1Hz)를 캐릭터 수준(100Hz)으로 끌어올려
 * 플레이어 상태 데이터를 빠르게 복제합니다.
 */
UCLASS()
class DDD_API ADDDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADDDPlayerState();

	/** 복제 대상 프로퍼티 등록 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UDDDInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDDInventoryComponent> InventoryComponent;
};
