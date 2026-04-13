// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthManagerComponent.h"

#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FBulletInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EBuffType BuffType = EBuffType::ENone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 bulletRangeMin = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 bulletRangeMax = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 cost = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DDD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBulletInfo> EquipBullets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBulletInfo> RandomBullets;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
