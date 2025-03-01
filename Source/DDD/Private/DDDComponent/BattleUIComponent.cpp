// Fill out your copyright notice in the Description page of Project Settings.


#include "DDDComponent/BattleUIComponent.h"

// Sets default values for this component's properties
UBattleUIComponent::UBattleUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBattleUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UBattleUIComponent::registerEnemy(int32 positionIndex, AActor* enemyActor, TArray<AActor*> enemyRegisterArray)
{
	if (positionIndex >= enemyRegisterArray.Num()) return false;
	if (enemyRegisterArray[positionIndex] != nullptr) return false;

	enemyRegisterArray[positionIndex] = enemyActor;
	UE_LOG(LogTemp, Warning, TEXT("EnemyName : %s"), *enemyActor->GetName());

	return true;
}

void UBattleUIComponent::FindEnemyToLock(int32 positionIndex, int defaultUnit, TArray<AActor*> enemyRegisterArray)
{
	int n = enemyRegisterArray.Num();
	if (defaultUnit == 0)
	{
		for (int i = 0; i < n; i++)
		{
			bridgeCallLockOnOff(i == 2, enemyRegisterArray[i]);
		}

		return;
	}

	positionIndex += defaultUnit;

	while (positionIndex >= 0 && positionIndex < n)
	{
		if (enemyRegisterArray[positionIndex] != nullptr)
		{
			for (int i=0; i< n; i++)
			{
				bridgeCallLockOnOff(i == positionIndex, enemyRegisterArray[i]);
			}
			
			return;
		}

		positionIndex += defaultUnit;
	}
	
}


// Called every frame
void UBattleUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

