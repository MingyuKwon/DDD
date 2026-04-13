// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DDDPlayerState.h"
#include "Net/UnrealNetwork.h"

ADDDPlayerState::ADDDPlayerState()
{
	// ACharacter 기본값(100.0f)에 맞춰 복제 주기 설정
	// APlayerState 기본값은 1.0f로 매우 낮음
	NetUpdateFrequency = 100.0f;
}

void ADDDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 추가 복제 프로퍼티는 여기에 DOREPLIFETIME 매크로로 등록
	// 예시: DOREPLIFETIME(ADDDPlayerState, SomeProperty);
}
