// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDDGameplayMode.generated.h"

class FFiniteStateMachine;

UENUM()
enum class EGameplayModeState : uint8
{
	Ready = 0,
	Begin,
	PlayerTurn,
	EnemyTurn,
	PlayerWin,
	EnemyWin,
	End,
	MAX
};
/**
 * 
 */
UCLASS()
class DDD_API ADDDGameplayMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 게임 시작 시 초기화
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	// 플레이어 로그인 완료 → 턴 FSM 시작 타이밍
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 레벨 시작 → BeginPlay 이후 FSM SetFirstState 타이밍
	virtual void StartPlay() override;

	// 플레이어 퇴장 시 FSM 정리
	virtual void Logout(AController* Exiting) override;

protected:
	TSharedPtr<FFiniteStateMachine> FSM;
	void InitFSM();
};
