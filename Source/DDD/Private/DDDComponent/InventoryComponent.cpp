// Fill out your copyright notice in the Description page of Project Settings.


#include "DDDComponent/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	for (int i=0; i<6; i++)
	{
		EquipBullets.Add(FBulletInfo());
		RandomBullets.Add(FBulletInfo());
	}

}


void UInventoryComponent::ReArrangeRandomArray()
{
	int32 size = RandomBullets.Num();

	for (int i = 0; i<size; i++)
	{
		if (RandomBullets[i].BuffType != EBuffType::ENone) continue;

		for (int j = i+1; j < size; j++)
		{
			if (RandomBullets[j].BuffType != EBuffType::ENone)
			{
				Swap(RandomBullets[i], RandomBullets[j]);
			}
		}

	}
}




