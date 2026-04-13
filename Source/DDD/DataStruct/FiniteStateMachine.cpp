#include "FiniteStateMachine.h"

DEFINE_LOG_CATEGORY(LogFSM);

void FFiniteStateMachine::Init()
{
	if (Description.IsEmpty())
	{
		Description = FString::Printf(TEXT("FSM[%p]"), this);
	}

	StatesMap.Empty();
	CurrentState.Reset();
	PrevStateID = FFiniteState::INVALID_STATE_ID;
	bDeferTransition = false;
}

TSharedPtr<FFiniteState> FFiniteStateMachine::RegisterState(uint8 StateID, TSharedPtr<FFiniteState> State)
{
	checkf(StateID != FFiniteState::INVALID_STATE_ID, TEXT("[%s] RegisterState: 유효하지 않은 StateID."), *Description);
	checkf(!StatesMap.Contains(StateID), TEXT("[%s] RegisterState: 이미 등록된 StateID(%u)."), *Description, StateID);

	if (!State.IsValid())
	{
		State = MakeShared<FFiniteState>(SharedThis(this));
	}

	State->Init(StateID);
	StatesMap.Add(StateID, State);

	return State;
}

bool FFiniteStateMachine::SetCurrentState(uint8 StateID)
{
	TSharedPtr<FFiniteState>* Found = StatesMap.Find(StateID);
	if (Found && Found->IsValid())
	{
		CurrentState = *Found;
		return true;
	}
	return false;
}

void FFiniteStateMachine::SetFirstState(uint8 StateID)
{
	StatesMap.Compact();
	StatesMap.Shrink();

	if (!SetCurrentState(StateID))
	{
		UE_LOG(LogFSM, Error, TEXT("[%s] SetFirstState: 등록되지 않은 StateID(%u)."), *Description, StateID);
		return;
	}

	{
		// Enter 도중 들어오는 전이 요청은 지연 처리
		TGuardValue<bool> ScopedDefer(bDeferTransition, true);
		TSharedRef<FFiniteStateMachine> This = SharedThis(this);

		checkf(CurrentState.IsValid(), TEXT("[%s] SetFirstState: CurrentState가 null."), *Description);
		CurrentState->OnEnter();
		OnStateChanged.ExecuteIfBound();
	}

	TransitStateIfPending();
}

bool FFiniteStateMachine::CanTransit(uint8 TargetStateID) const
{
	if (!CurrentState.IsValid())
	{
		UE_LOG(LogFSM, Warning, TEXT("[%s] CanTransit: CurrentState가 null."), *Description);
		return false;
	}
	return CurrentState->CanTransitTo(TargetStateID);
}

bool FFiniteStateMachine::TransitState(uint8 TargetStateID)
{
	if (!CanTransit(TargetStateID))
	{
		UE_LOG(LogFSM, Verbose, TEXT("[%s] TransitState: [%u] → [%u] 전이 불가."),
			*Description, CurrentState.IsValid() ? CurrentState->GetStateID() : FFiniteState::INVALID_STATE_ID, TargetStateID);
		return false;
	}

	PendingTransitions.Enqueue(TargetStateID);
	return TransitStateIfPending();
}

bool FFiniteStateMachine::TransitStateIfPending()
{
	if (PendingTransitions.IsEmpty())
	{
		return true;
	}

	if (bDeferTransition)
	{
		// 보류 중인 전이가 나중에 처리된다고 가정하고 true 반환
		return true;
	}

	TGuardValue<bool> ScopedDefer(bDeferTransition, true);

	// 한 프레임에 너무 많은 전이가 연속되는 것을 방지
	constexpr int32 TransitionLimit = 16;
	int32 TransitionCount = 0;
	bool bSuccess = true;

	uint8 TargetStateID;
	while (PendingTransitions.Dequeue(TargetStateID))
	{
		if (!CanTransit(TargetStateID))
		{
			UE_LOG(LogFSM, Warning, TEXT("[%s] TransitStateIfPending: [%u] → [%u] 전이 조건 불충족."),
				*Description, CurrentState.IsValid() ? CurrentState->GetStateID() : FFiniteState::INVALID_STATE_ID, TargetStateID);
			bSuccess = false;
			break;
		}

		if (!TransitStateInternal(TargetStateID))
		{
			bSuccess = false;
			break;
		}

		++TransitionCount;
		if (TransitionCount >= TransitionLimit)
		{
			ensureMsgf(false, TEXT("[%s] TransitStateIfPending: 한 프레임에 너무 많은 전이 발생. 마지막 목적지 [%u]."),
				*Description, TargetStateID);
			bSuccess = false;
			break;
		}
	}

	if (!bSuccess)
	{
		PendingTransitions.Empty();
	}

	return bSuccess;
}

bool FFiniteStateMachine::TransitStateInternal(uint8 TargetStateID)
{
	const uint8 FromStateID = CurrentState->GetStateID();

	TSharedRef<FFiniteState> PrevState = CurrentState.ToSharedRef();

	if (!SetCurrentState(TargetStateID))
	{
		UE_LOG(LogFSM, Error, TEXT("[%s] TransitStateInternal: StateID(%u) 로 전환 실패."), *Description, TargetStateID);
		return false;
	}

	UE_LOG(LogFSM, Verbose, TEXT("[%s] 전이: [%u] → [%u]"), *Description, FromStateID, TargetStateID);

	// 호출 중 자신의 메모리가 해제되지 않도록 참조 유지
	TSharedRef<FFiniteStateMachine> This = SharedThis(this);

	PrevStateID = FromStateID;
	PrevState->OnExit();

	CurrentState->OnEnter();
	OnStateChanged.ExecuteIfBound();

	return true;
}

void FFiniteStateMachine::UpdateState(float DeltaSeconds)
{
	if (!CurrentState.IsValid())
	{
		return;
	}

	{
		// Update 도중 전이 요청은 지연 처리
		TGuardValue<bool> ScopedDefer(bDeferTransition, true);
		TSharedRef<FFiniteStateMachine> This = SharedThis(this);
		CurrentState->OnUpdate(DeltaSeconds);
	}

	TransitStateIfPending();
}
