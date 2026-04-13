// Fill out your copyright notice in the Description page of Project Settings.


#include "DDDFunctionLibrary.h"

int32 UDDDFunctionLibrary::CalcResultInt32(int32 leftOperand, int32 rightOperand, EBuffType buffType)
{
	switch (buffType)
	{
		case EBuffType::EPlus :
			leftOperand += rightOperand;
			break;
		case EBuffType::EMinus:
			leftOperand -= rightOperand;
			break;

		case EBuffType::EMultiple:
			leftOperand *= rightOperand;
			break;

		case EBuffType::EDivide:
			if (rightOperand != 0)
			{
				leftOperand /= rightOperand;
			}
			break;
	}

	return leftOperand;
}
