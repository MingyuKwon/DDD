#pragma once

#include <type_traits>
#include "CoreMinimal.h"

class FFiniteStateMachine;

/**
 * FSM 의 개별 상태를 나타내는 클래스.
 * 서브클래싱하거나 델리게이트 바인딩으로 Enter/Update/Exit 로직을 주입할 수 있음.
 *
 * 사용 예:
 *   TSharedPtr<FFiniteState> IdleState = FSM->RegisterState(ECharacterState::Idle);
 *   IdleState->AddTransition(ECharacterState::Run);
 *   IdleState->BindOnEnter(this, &AMyCharacter::OnEnterIdle);
 *   IdleState->BindOnExit(this, &AMyCharacter::OnExitIdle);
 */
class FFiniteState : public TSharedFromThis<FFiniteState>
{
	DECLARE_DELEGATE(FStateDelegate)
	DECLARE_DELEGATE_OneParam(FStateUpdateDelegate, float)

public:
	explicit  FFiniteState(const TSharedPtr<FFiniteStateMachine>& OwnerMachine) : OwnerMachine(OwnerMachine) {}
	virtual ~FFiniteState() {}

	static constexpr uint8 INVALID_STATE_ID = MAX_uint8;

	void Init(uint8 InStateID);
	uint8 GetStateID() const { return StateID; }

	/** 이 상태에서 전이 가능한 목적지 상태를 등록 */
	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	void AddTransition(const T InStateID) { AddTransition(static_cast<uint8>(InStateID)); }

	bool CanTransitTo(uint8 TargetStateID) const { return Transitions.Contains(TargetStateID); }

	/** UObject 메서드 바인딩 */
	template<typename UserClass, typename FunctorType>
	void BindOnEnter(UserClass* Obj, FunctorType&& Func)
	{
		OnEnterDelegate.BindUObject(Obj, Forward<FunctorType>(Func));
	}

	template<typename UserClass, typename FunctorType>
	void BindOnUpdate(UserClass* Obj, FunctorType&& Func)
	{
		OnUpdateDelegate.BindUObject(Obj, Forward<FunctorType>(Func));
	}

	template<typename UserClass, typename FunctorType>
	void BindOnExit(UserClass* Obj, FunctorType&& Func)
	{
		OnExitDelegate.BindUObject(Obj, Forward<FunctorType>(Func));
	}

	void SetUpdateEnabled(bool bEnabled) { bUpdateEnabled = bEnabled; }
	bool IsUpdateEnabled() const { return bUpdateEnabled; }

	/** 현재 상태에서 TargetState 로 전이 요청. 허용된 Transition 이 없으면 실패. */
	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	bool TransitState(const T TargetState) { return TransitState(static_cast<uint8>(TargetState)); }

protected:
	/** 서브클래스에서 오버라이드 가능 */
	virtual void OnEnterImpl() {}
	virtual void OnUpdateImpl(float DeltaSeconds) {}
	virtual void OnExitImpl() {}


private:
	void OnEnter();
	void OnUpdate(float DeltaSeconds);
	void OnExit();
	void AddTransition(uint8 InStateID);

	bool TransitState(uint8 State) const;

private:
	uint8 StateID = INVALID_STATE_ID;
	TSet<uint8> Transitions;
	bool bUpdateEnabled = true;

	FStateDelegate OnEnterDelegate;
	FStateUpdateDelegate OnUpdateDelegate;
	FStateDelegate OnExitDelegate;

	TWeakPtr<FFiniteStateMachine> OwnerMachine;

	friend FFiniteStateMachine;
};
