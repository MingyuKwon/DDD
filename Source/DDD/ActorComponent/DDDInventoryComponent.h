#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/DDDGameplayTags.h"
#include "DDDInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FBulletCardEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag BulletType;

    UPROPERTY(BlueprintReadOnly)
    int32 Scale = 0;

    bool IsValid() const
    {
        return BulletType.IsValid() && Scale > 0;
	}

    bool operator==(const FBulletCardEntry& Other) const
    {
		return BulletType == Other.BulletType && Scale == Other.Scale;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandIndexChanged, int32, index);

UCLASS(ClassGroup = "DDD", meta = (BlueprintSpawnableComponent))
class DDD_API UDDDInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDDDInventoryComponent();

#pragma region 
    UFUNCTION(Server, Reliable)
    void ServerAddBulletToDeck(const FGameplayTag& BulletType, int32 Scale);
    UFUNCTION(Server, Reliable)
    void ServerRemoveBulletFromDeck(const FGameplayTag& BulletType, int32 Scale);

    UFUNCTION(Server, Reliable)
    void ServerAddBulletToHand(const FGameplayTag& BulletType, int32 Scale);
    UFUNCTION(Server, Reliable)
    void ServerRemoveBulletFromHand(const FGameplayTag& BulletType, int32 Scale);

    UFUNCTION(Server, Reliable)
    void ServerRemoveCurrentBullet(bool bDamageHand);

    UFUNCTION(Server, Reliable)
    void ServerRotateHand(bool bDamageHand, bool bClockwise);
#pragma endregion

    FBulletCardEntry GetCurrentBullet(bool bDamageHand);
    bool RemoveCurrentBullet(bool bDamageHand);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FSimpleMulticastDelegate OnDamageDeckChanged;
	FSimpleMulticastDelegate OnBuffDeckChanged;
	FSimpleMulticastDelegate OnDamageHandChanged;
	FSimpleMulticastDelegate OnBuffHandChanged;

	FHandIndexChanged OnDamageHandIndexChanged;
	FHandIndexChanged OnBuffHandIndexChanged;

private:
    bool IsBulletTypeDamage(const FGameplayTag& BulletType) const { return BulletType == TAG_Bullet_Damage; }
	bool HasAuthority() const { return GetOwnerRole() == ROLE_Authority; }
#pragma region
    void AddBulletToDeck_Internal(const FGameplayTag& BulletType, int32 Scale);
    bool RemoveBulletFromDeck_Internal(const FGameplayTag& BulletType, int32 Scale);

    void AddBulletToHand_Internal(const FGameplayTag& BulletType, int32 Scale);
    bool RemoveBulletFromHand_Internal(const FGameplayTag& BulletType, int32 Scale);

    bool RemoveCurrentBullet_Internal(bool bDamageHand);

    void RotateHand_Internal(bool bDamageHand, bool bClockwise);
#pragma endregion

    UPROPERTY(ReplicatedUsing = OnRep_DamageBulletDeck)
    TArray<FBulletCardEntry> DamageBulletDeck;
    UPROPERTY(ReplicatedUsing = OnRep_DamageBulletHand)
    TArray<FBulletCardEntry> DamageBulletHand;
    UPROPERTY(ReplicatedUsing = OnRep_CurrentDamageBulletIndex)
    int32 CurrentDamageBulletIndex = 0;

    UPROPERTY(ReplicatedUsing = OnRep_BuffBulletDeck)
    TArray<FBulletCardEntry> BuffBulletDeck;
    UPROPERTY(ReplicatedUsing = OnRep_BuffBulletHand)
    TArray<FBulletCardEntry> BuffBulletHand;
    UPROPERTY(ReplicatedUsing = OnRep_CurrentBuffBulletIndex)
    int32 CurrentBuffBulletIndex = 0;

    UFUNCTION()
    void OnRep_DamageBulletDeck();
    UFUNCTION()
    void OnRep_DamageBulletHand();
    UFUNCTION()
    void OnRep_CurrentDamageBulletIndex();

    UFUNCTION()
    void OnRep_BuffBulletDeck();
	UFUNCTION()
    void OnRep_BuffBulletHand();
    UFUNCTION()
    void OnRep_CurrentBuffBulletIndex();
};
