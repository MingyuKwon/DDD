#include "GameMode/DDDGameplayModeState.h"

#include "DDDGameplayMode.h"
#include "DDDLog.h"
#include "DataStruct/FiniteStateMachine.h"

void FDDDGameplayModeState::OnEnterImpl()
{
	FFiniteState::OnEnterImpl();
	DDD_LOG(LogFSM, Log, TEXT("Entering State: %s"), *FStringFunctionLibrary::EnumToString(static_cast<EGameplayModeState>(GetStateID())));
}

void FDDDGameplayModeState::OnExitImpl()
{
	FFiniteState::OnExitImpl();
	DDD_LOG(LogFSM, Log, TEXT("Exiting State: %s"), *FStringFunctionLibrary::EnumToString(static_cast<EGameplayModeState>(GetStateID())));
}

// FDDDGameplayModeState_Begin implementation 
void FDDDGameplayModeState_Begin::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
	TransitState(EGameplayModeState::PlayerTurn);
}

void FDDDGameplayModeState_Begin::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_Begin::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_Begin implementation 

// FDDDGameplayModeState_PlayerTurn implementation 
void FDDDGameplayModeState_PlayerTurn::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
}

void FDDDGameplayModeState_PlayerTurn::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_PlayerTurn::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_PlayerTurn implementation 

// FDDDGameplayModeState_EnemyTurn implementation 
void FDDDGameplayModeState_EnemyTurn::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
}

void FDDDGameplayModeState_EnemyTurn::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_EnemyTurn::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_EnemyTurn implementation 

// FDDDGameplayModeState_PlayerWin implementation 
void FDDDGameplayModeState_PlayerWin::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
}

void FDDDGameplayModeState_PlayerWin::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_PlayerWin::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_PlayerWin implementation 

// FDDDGameplayModeState_EnemyWin implementation 
void FDDDGameplayModeState_EnemyWin::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
}

void FDDDGameplayModeState_EnemyWin::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_EnemyWin::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_EnemyWin implementation 

// FDDDGameplayModeState_End implementation 
void FDDDGameplayModeState_End::OnEnterImpl()
{
	FDDDGameplayModeState::OnEnterImpl();
}

void FDDDGameplayModeState_End::OnUpdateImpl(float DeltaSeconds)
{
	FDDDGameplayModeState::OnUpdateImpl(DeltaSeconds);
}

void FDDDGameplayModeState_End::OnExitImpl()
{
	FDDDGameplayModeState::OnExitImpl();
}
// FDDDGameplayModeState_End implementation 