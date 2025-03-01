// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DDD_API UBattleUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBattleUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual bool registerEnemy(int32 positionIndex, AActor* enemyActor, UPARAM(ref) TArray<AActor*>& enemyRegisterArray);

	UFUNCTION(BlueprintCallable)
	virtual void FindEnemyToLock(UPARAM(ref) int32& positionIndex, int defaultUnit, const TArray<AActor*>& enemyRegisterArray);

	UFUNCTION(BlueprintImplementableEvent)
	void bridgeCallLockOnOff(bool bLockOn, AActor* enemyActor);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
