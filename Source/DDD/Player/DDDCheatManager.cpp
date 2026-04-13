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
	// 짧은 이름(예: "Damage")을 입력하면 자동으로 "DDD.Bullet." 프리픽스를 붙임
	const FString FullName = TagName.Contains(TEXT("."))
		? TagName
		: FString::Printf(TEXT("DDD.Bullet.%s"), *TagName);

	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*FullName), false);
	if (!Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Cheat] 유효하지 않은 태그: '%s'. 사용 가능: Damage / Minus / Modulo / Multiply"),
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

void UDDDCheatManager::Inv_SetCurrentIndex(bool bDamageHand, int32 NewIndex)
{
	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		Inv->ServerSetCurrentIndex(bDamageHand, NewIndex);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] CurrentIndex 설정 (DamageHand=%s, Index=%d)"),
			bDamageHand ? TEXT("true") : TEXT("false"), NewIndex);
	}
}

void UDDDCheatManager::Inv_ShiftIndex(bool bDamageHand, bool bForward)
{
	if (UDDDInventoryComponent* Inv = GetInventory())
	{
		const int32 Num = Inv->GetHandNum(bDamageHand);
		if (Num <= 0) return;

		const int32 Current = Inv->GetCurrentBulletIndex(bDamageHand);
		const int32 Next = (Current + (bForward ? 1 : -1) + Num) % Num;
		Inv->ServerSetCurrentIndex(bDamageHand, Next);
		UE_LOG(LogTemp, Log, TEXT("[Cheat] Index %d → %d (DamageHand=%s)"),
			Current, Next, bDamageHand ? TEXT("true") : TEXT("false"));
	}
}
