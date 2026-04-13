// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DDDCheatManager.generated.h"

class UDDDInventoryComponent;

/**
 * DDDCheatManager
 *
 * 콘솔 명령어로 로컬 플레이어의 인벤토리를 조작합니다.
 * PlayerController에 CheatClass로 등록해서 사용합니다.
 *
 * 사용 가능한 태그:
 *   DDD.Bullet.Damage / DDD.Bullet.Minus / DDD.Bullet.Modulo / DDD.Bullet.Multiply
 *
 * 콘솔 명령어 예시:
 *   Inv_AddToDeck DDD.Bullet.Damage 3
 *   Inv_AddToHand DDD.Bullet.Minus 1
 *   Inv_RotateHand true true
 */
UCLASS()
class DDD_API UDDDCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	// --- 덱(Deck) 조작 ---

	/** 덱에 총알 추가. TagName: DDD.Bullet.Damage 등 */
	UFUNCTION(exec)
	void Inv_AddToDeck(const FString& TagName, int32 Scale = 1);

	/** 덱에서 총알 제거 */
	UFUNCTION(exec)
	void Inv_RemoveFromDeck(const FString& TagName, int32 Scale = 1);

	// --- 핸드(Hand) 조작 ---

	/** 핸드에 총알 추가 */
	UFUNCTION(exec)
	void Inv_AddToHand(const FString& TagName, int32 Scale = 1);

	/** 핸드에서 총알 제거 */
	UFUNCTION(exec)
	void Inv_RemoveFromHand(const FString& TagName, int32 Scale = 1);

	/** 현재 선택된 총알 제거. bDamageHand=true면 데미지 핸드 */
	UFUNCTION(exec)
	void Inv_RemoveCurrentBullet(bool bDamageHand);

	/** 핸드 순환. bClockwise=true면 시계 방향 */
	UFUNCTION(exec)
	void Inv_RotateHand(bool bDamageHand, bool bClockwise);

private:

	/** 로컬 플레이어의 InventoryComponent 반환. 없으면 nullptr + 경고 로그 */
	UDDDInventoryComponent* GetInventory() const;

	/** TagName 문자열 → FGameplayTag 변환. 유효하지 않으면 경고 로그 후 빈 태그 반환 */
	FGameplayTag ParseTag(const FString& TagName) const;
};
