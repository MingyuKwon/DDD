// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DDDGameplayMode.h"

#include "DataStruct/FiniteStateMachine.h"
#include "GameMode/DDDGameplayModeState.h"

void ADDDGameplayMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	InitFSM();
}

void ADDDGameplayMode::InitGameState()
{
	Super::InitGameState();
}

void ADDDGameplayMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADDDGameplayMode::StartPlay()
{
	Super::StartPlay();
	FSM->TransitState(EGameplayModeState::Begin);
}

void ADDDGameplayMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ADDDGameplayMode::InitFSM()
{
	FSM = FFiniteStateMachine::Create();
	{
		TSharedPtr<FFiniteState> ReadyState = FSM->RegisterState(EGameplayModeState::Ready);
		ReadyState->AddTransition(EGameplayModeState::Begin);
		
		TSharedPtr<FFiniteState> BeginState = FSM->RegisterState(EGameplayModeState::Begin, MakeShared<FDDDGameplayModeState_Begin>(FSM));
		BeginState->AddTransition(EGameplayModeState::PlayerTurn);
		BeginState->AddTransition(EGameplayModeState::EnemyTurn);

		TSharedPtr<FFiniteState> PlayerTurnState = FSM->RegisterState(EGameplayModeState::PlayerTurn, MakeShared<FDDDGameplayModeState_PlayerTurn>(FSM));
		PlayerTurnState->AddTransition(EGameplayModeState::EnemyTurn);
		PlayerTurnState->AddTransition(EGameplayModeState::PlayerWin);
		PlayerTurnState->AddTransition(EGameplayModeState::EnemyWin);

		TSharedPtr<FFiniteState> EnemyTurnState = FSM->RegisterState(EGameplayModeState::EnemyTurn, MakeShared<FDDDGameplayModeState_EnemyTurn>(FSM));
		EnemyTurnState->AddTransition(EGameplayModeState::PlayerTurn);
		EnemyTurnState->AddTransition(EGameplayModeState::EnemyTurn);
		PlayerTurnState->AddTransition(EGameplayModeState::PlayerWin);
		PlayerTurnState->AddTransition(EGameplayModeState::EnemyWin);

		TSharedPtr<FFiniteState> PlayerWinState = FSM->RegisterState(EGameplayModeState::PlayerWin, MakeShared<FDDDGameplayModeState_PlayerWin>(FSM));
		PlayerWinState->AddTransition(EGameplayModeState::End);

		TSharedPtr<FFiniteState> EnemyWinState = FSM->RegisterState(EGameplayModeState::EnemyWin, MakeShared<FDDDGameplayModeState_EnemyWin>(FSM));
		EnemyWinState->AddTransition(EGameplayModeState::End);

		TSharedPtr<FFiniteState> EndState = FSM->RegisterState(EGameplayModeState::End, MakeShared<FDDDGameplayModeState_End>(FSM));
	}

	FSM->SetFirstState(EGameplayModeState::Ready);
}
