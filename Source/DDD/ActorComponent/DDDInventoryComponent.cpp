#include "ActorComponent/DDDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UDDDInventoryComponent::UDDDInventoryComponent()
{
    SetIsReplicatedByDefault(true);
}

void UDDDInventoryComponent::AddBulletToDeck_Internal(const FGameplayTag& BulletType, int32 Scale)
{
	FBulletCardEntry BulletEntry;
    BulletEntry.BulletType = BulletType;
    BulletEntry.Scale = Scale;  

    if (BulletType == TAG_Bullet_Damage)
    {
        DamageBulletDeck.Add(BulletEntry);
    }
    else
    {
        BuffBulletDeck.Add(BulletEntry);
    }
}

bool UDDDInventoryComponent::RemoveBulletFromDeck_Internal(const FGameplayTag& BulletType, int32 Scale)
{
    FBulletCardEntry BulletEntry;
    BulletEntry.BulletType = BulletType;
    BulletEntry.Scale = Scale;

    if (BulletType == TAG_Bullet_Damage)
    {
        return DamageBulletDeck.RemoveSingle(BulletEntry) > 0;
    }
    else
    {
        return BuffBulletDeck.RemoveSingle(BulletEntry) > 0;
	}
}

void UDDDInventoryComponent::AddBulletToHand_Internal(const FGameplayTag& BulletType, int32 Scale)
{
	bool bIsDamageBullet = BulletType == TAG_Bullet_Damage;

    FBulletCardEntry BulletEntry;
    BulletEntry.BulletType = BulletType;
    BulletEntry.Scale = Scale;  

    if (bIsDamageBullet)
    {
        DamageBulletHand.Add(BulletEntry);
    }
    else
    {
        BuffBulletHand.Add(BulletEntry);
	}
}

bool UDDDInventoryComponent::RemoveBulletFromHand_Internal(const FGameplayTag& BulletType, int32 Scale)
{
    bool bIsDamageBullet = BulletType == TAG_Bullet_Damage;

    FBulletCardEntry BulletEntry;
    BulletEntry.BulletType = BulletType;
    BulletEntry.Scale = Scale;

    if (bIsDamageBullet)
    {
        return DamageBulletHand.RemoveSingle(BulletEntry) > 0;
    }
    else
    {
        return BuffBulletHand.RemoveSingle(BulletEntry) > 0;
    }
}

bool UDDDInventoryComponent::RemoveCurrentBullet_Internal(bool bDamageHand)
{
    TArray<FBulletCardEntry>& Hand = bDamageHand ? DamageBulletHand : BuffBulletHand;
    int32& CurrentIndex = bDamageHand ? CurrentDamageBulletIndex : CurrentBuffBulletIndex;
    if (Hand.IsValidIndex(CurrentIndex))
    {
        Hand.RemoveAt(CurrentIndex);
        // Adjust current index if needed
        if (CurrentIndex >= Hand.Num())
        {
            CurrentIndex = 0;
        }
        return true;
    }
	return false;
}

void UDDDInventoryComponent::RotateHand_Internal(bool bDamageHand, bool bClockwise)
{
    TArray<FBulletCardEntry>& Hand = bDamageHand ? DamageBulletHand : BuffBulletHand;
	int32& CurrentIndex = bDamageHand ? CurrentDamageBulletIndex : CurrentBuffBulletIndex;

    int32 MaxHand = Hand.Num();
    if (bClockwise)
    {
        CurrentIndex = (CurrentIndex + 1) % MaxHand;
    }
    else
    {
        CurrentIndex = (CurrentIndex - 1 + MaxHand) % MaxHand;
	}
}

void UDDDInventoryComponent::ServerRemoveCurrentBullet_Implementation(bool bDamageHand)
{
    if (RemoveCurrentBullet_Internal(bDamageHand))
    {
        if (bDamageHand)
        {
            OnRep_DamageBulletHand();
            OnRep_CurrentDamageBulletIndex();
        }
        else
        {
            OnRep_BuffBulletHand();
            OnRep_CurrentBuffBulletIndex();
        }
	}
}

FBulletCardEntry UDDDInventoryComponent::GetCurrentBullet(bool bDamageHand)
{
	TArray<FBulletCardEntry>& Hand = bDamageHand ? DamageBulletHand : BuffBulletHand;
	int32 CurrentIndex = bDamageHand ? CurrentDamageBulletIndex : CurrentBuffBulletIndex;

    if (Hand.IsValidIndex(CurrentIndex))
    {
        return Hand[CurrentIndex];
    }

	return FBulletCardEntry();
}

bool UDDDInventoryComponent::RemoveCurrentBullet(bool bDamageHand)
{
    TArray<FBulletCardEntry>& Hand = bDamageHand ? DamageBulletHand : BuffBulletHand;
    int32& CurrentIndex = bDamageHand ? CurrentDamageBulletIndex : CurrentBuffBulletIndex;
    if (Hand.IsValidIndex(CurrentIndex))
    {
        Hand.RemoveAt(CurrentIndex);
        // Adjust current index if needed
        if (CurrentIndex >= Hand.Num())
        {
            CurrentIndex = 0;
        }
        return true;
    }
	return false;
}

void UDDDInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UDDDInventoryComponent, DamageBulletDeck);
    DOREPLIFETIME(UDDDInventoryComponent, DamageBulletHand);
    DOREPLIFETIME(UDDDInventoryComponent, BuffBulletDeck);
    DOREPLIFETIME(UDDDInventoryComponent, BuffBulletHand);
    DOREPLIFETIME(UDDDInventoryComponent, CurrentDamageBulletIndex);
    DOREPLIFETIME(UDDDInventoryComponent, CurrentBuffBulletIndex);
}

void UDDDInventoryComponent::ServerRemoveBulletFromHand_Implementation(const FGameplayTag& BulletType, int32 Scale)
{
	RemoveBulletFromHand_Internal(BulletType, Scale);

    if (IsBulletTypeDamage(BulletType))
    {
        OnRep_DamageBulletHand();
    }
    else
    {
        OnRep_BuffBulletHand();
    }
}

void UDDDInventoryComponent::ServerAddBulletToHand_Implementation(const FGameplayTag& BulletType, int32 Scale)
{
    AddBulletToHand_Internal(BulletType, Scale);

    if (IsBulletTypeDamage(BulletType))
    {
        OnRep_DamageBulletHand();
    }
    else
    {
        OnRep_BuffBulletHand();
    }
}

void UDDDInventoryComponent::ServerRemoveBulletFromDeck_Implementation(const FGameplayTag& BulletType, int32 Scale)
{
	RemoveBulletFromDeck_Internal(BulletType, Scale);

    if (IsBulletTypeDamage(BulletType))
    {
        OnRep_DamageBulletDeck();
    }
    else
    {
        OnRep_BuffBulletDeck();
    }
}

void UDDDInventoryComponent::ServerAddBulletToDeck_Implementation(const FGameplayTag& BulletType, int32 Scale)
{
	AddBulletToDeck_Internal(BulletType, Scale);

    if(IsBulletTypeDamage(BulletType))
    {
        OnRep_DamageBulletDeck();
    }
    else
    {
        OnRep_BuffBulletDeck();
    }
}

void UDDDInventoryComponent::ServerRotateHand_Implementation(bool bDamageHand, bool bClockwise)
{
    RotateHand_Internal(bDamageHand, bClockwise);

    if(bDamageHand)
    {
        OnRep_CurrentDamageBulletIndex();
    }
    else
    {
        OnRep_CurrentBuffBulletIndex();
	}
}

void UDDDInventoryComponent::OnRep_DamageBulletDeck()
{
    OnDamageDeckChanged.Broadcast();
}

void UDDDInventoryComponent::OnRep_DamageBulletHand()
{
    OnDamageHandChanged.Broadcast();
}

void UDDDInventoryComponent::OnRep_CurrentDamageBulletIndex()
{
    OnDamageHandIndexChanged.Broadcast(CurrentDamageBulletIndex);
}

void UDDDInventoryComponent::OnRep_BuffBulletDeck()
{
    OnBuffDeckChanged.Broadcast();
}

void UDDDInventoryComponent::OnRep_BuffBulletHand()
{
    OnBuffHandChanged.Broadcast();
}

void UDDDInventoryComponent::OnRep_CurrentBuffBulletIndex()
{
    OnBuffHandIndexChanged.Broadcast(CurrentBuffBulletIndex);
}
