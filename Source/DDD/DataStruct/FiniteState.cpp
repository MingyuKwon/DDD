#include "FiniteState.h"

#include "FiniteStateMachine.h"

void FFiniteState::Init(uint8 InStateID)
{
	StateID = InStateID;
	Transitions.Reset();
}

void FFiniteState::AddTransition(uint8 InStateID)
{
	if (InStateID != INVALID_STATE_ID)
	{
		Transitions.Add(InStateID);
	}
}

bool FFiniteState::TransitState(uint8 State) const
{
	check(OwnerMachine.IsValid());

	TSharedPtr<FFiniteStateMachine> Machine = OwnerMachine.Pin();
	return Machine->TransitState(State);
}

void FFiniteState::OnEnter()
{
	// 호출 중 자신의 메모리가 해제되지 않도록 참조 유지
	TSharedRef<FFiniteState> This = SharedThis(this);
	OnEnterDelegate.ExecuteIfBound();
	OnEnterImpl();
}

void FFiniteState::OnUpdate(float DeltaSeconds)
{
	if (!bUpdateEnabled)
	{
		return;
	}

	TSharedRef<FFiniteState> This = SharedThis(this);
	OnUpdateDelegate.ExecuteIfBound(DeltaSeconds);
	OnUpdateImpl(DeltaSeconds);
}

void FFiniteState::OnExit()
{
	TSharedRef<FFiniteState> This = SharedThis(this);
	OnExitImpl();
	OnExitDelegate.ExecuteIfBound();
}
