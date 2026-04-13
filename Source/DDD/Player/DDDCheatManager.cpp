// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DDDCheatManager.h"
#include "Player/DDDPlayerState.h"
#include "ActorComponent/DDDInventoryComponent.h"
#include "GameplayTagsManager.h"

// ─────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────

UDDDInventoryComponent* UDDDCheatManager::GetInventory() const
{
	APlayerController* PC = GetOuterAPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Cheat] PlayerController를 찾을 수 없습니다."));
		return nullptr;
	}

	ADDDPlayerState* PS = PC->GetPlayerState<ADDDPlayerState>();
	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Cheat] DDDPlayerState를 찾을 수 없습니다."));
		return nullptr;
	}

	UDDDInventoryComponent* Inv = PS->GetInventoryComponent();
	if (!Inv)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Cheat] InventoryComponent를 찾을 수 없습니다."));
	}
	return Inv;
}

FGameplayTag UDDDCheatManager::ParseTag(const FString& TagName) const
{
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Cheat] 유효하지 않은 태그: '%s'. 사용 가능: DDD.Bullet.Damage / DDD.Bullet.Minus / DDD.Bullet.Modulo / DDD.Bullet.Multiply"),
			*TagName);
	}
	return Tag;
}

// ─────────────────────────────────────────────
// Deck
// ─────────────────────────────────────────────

void UDDDCheatManager::Inv_AddToDeck(const FString& TagName, int32 Scale)
{
	FGameplayTag Tag = ParseTag(TagName);
	if (!Tag.IsValid()) return;

	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerAddBulletToDeck(Tag, Scale);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Deck에 추가: %s x%d"), *TagName, Scale);
	}
}

void UDDDCheatManager::Inv_RemoveFromDeck(const FString& TagName, int32 Scale)
{
	FGameplayTag Tag = ParseTag(TagName);
	if (!Tag.IsValid()) return;

	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerRemoveBulletFromDeck(Tag, Scale);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Deck에서 제거: %s x%d"), *TagName, Scale);
	}
}

// ─────────────────────────────────────────────
// Hand
// ─────────────────────────────────────────────

void UDDDCheatManager::Inv_AddToHand(const FString& TagName, int32 Scale)
{
	FGameplayTag Tag = ParseTag(TagName);
	if (!Tag.IsValid()) return;

	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerAddBulletToHand(Tag, Scale);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Hand에 추가: %s x%d"), *TagName, Scale);
	}
}

void UDDDCheatManager::Inv_RemoveFromHand(const FString& TagName, int32 Scale)
{
	FGameplayTag Tag = ParseTag(TagName);
	if (!Tag.IsValid()) return;

	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerRemoveBulletFromHand(Tag, Scale);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Hand에서 제거: %s x%d"), *TagName, Scale);
	}
}

void UDDDCheatManager::Inv_RemoveCurrentBullet(bool bDamageHand)
{
	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerRemoveCurrentBullet(bDamageHand);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] 현재 총알 제거 (DamageHand=%s)"), bDamageHand ? TEXT("true") : TEXT("false"));
	}
}

void UDDDCheatManager::Inv_RotateHand(bool bDamageHand, bool bClockwise)
{
	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerRotateHand(bDamageHand, bClockwise);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Hand 회전 (DamageHand=%s, Clockwise=%s)"),
			bDamageHand ? TEXT("true") : TEXT("false"),
			bClockwise  ? TEXT("true") : TEXT("false"));
	}
}
