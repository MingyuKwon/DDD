// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DDD/DDD.h"

#include "DDDFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DDD_API UDDDFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DDD Library")
	static int32 CalcResultInt32(int32 leftOperand, int32 rightOperand, EBuffType buffType);

};
