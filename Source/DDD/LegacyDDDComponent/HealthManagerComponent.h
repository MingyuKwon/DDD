// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DDD.h"


#include "HealthManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FBuff
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EBuffType BuffType = EBuffType::ENone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Operand = 0;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DDD_API UHealthManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 currentHealth = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 maxHealth = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBuff> buffStack;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
