#pragma once

#include <type_traits>
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "FiniteState.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFSM, Log, All);

/**
 * enum class(uint8) 기반의 유한 상태 기계(FSM).
 *
 * 사용 예:
 *   TSharedPtr<FFiniteStateMachine> FSM = FFiniteStateMachine::Create(TEXT("PlayerFSM"));
 *   {
 *       TSharedPtr<FFiniteState> IdleState = FSM->RegisterState(ECharacterState::Idle);
 *       IdleState->AddTransition(ECharacterState::Run);
 *       IdleState->AddTransition(ECharacterState::Attack);
 *       IdleState->BindOnEnter(this, &AMyCharacter::OnEnterIdle);
 *       IdleState->BindOnExit(this, &AMyCharacter::OnExitIdle);
 *
 *       TSharedPtr<FFiniteState> RunState = FSM->RegisterState(ECharacterState::Run);
 *       RunState->AddTransition(ECharacterState::Idle);
 *       RunState->BindOnUpdate(this, &AMyCharacter::OnUpdateRun);
 *   }
 *   FSM->SetFirstState(ECharacterState::Idle);
 *
 *   // Tick 에서:
 *   FSM->UpdateState(DeltaSeconds);
 *
 *   // 상태 전이:
 *   FSM->TransitState(ECharacterState::Run);
 */
class FFiniteStateMachine : public TSharedFromThis<FFiniteStateMachine>
{
public:
	/**
	 * FSM 인스턴스를 생성한다.
	 * 서브클래스를 만들 경우 템플릿 인자로 지정 가능.
	 */
	template<typename THandler = FFiniteStateMachine>
	static TSharedPtr<THandler> Create(const FString& Description = FString())
	{
		TSharedRef<THandler> Machine = MakeShared<THandler>();
		Machine->SetDescription(Description);
		Machine->Init();
		return Machine;
	}

	FFiniteStateMachine() = default;
	virtual ~FFiniteStateMachine() {}

	/** 상태 등록. State 를 nullptr 로 주면 기본 FFiniteState 를 자동 생성. */
	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	TSharedPtr<FFiniteState> RegisterState(const T StateID, TSharedPtr<FFiniteState> State = nullptr)
	{
		return RegisterState(static_cast<uint8>(StateID), MoveTemp(State));
	}

	/** 초기 상태를 설정하고 Enter 를 호출한다. RegisterState 후 반드시 한 번 호출. */
	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	void SetFirstState(const T StateID) { SetFirstState(static_cast<uint8>(StateID)); }

	/** 현재 상태에서 TargetState 로 전이 요청. 허용된 Transition 이 없으면 실패. */
	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	bool TransitState(const T TargetState) { return TransitState(static_cast<uint8>(TargetState)); }

	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	T GetCurrentStateID() const
	{
		if (CurrentState.IsValid())
		{
			return static_cast<T>(CurrentState->GetStateID());
		}
		return static_cast<T>(FFiniteState::INVALID_STATE_ID);
	}

	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	T GetPrevStateID() const { return static_cast<T>(PrevStateID); }

	/** 현재 상태를 특정 서브클래스로 캐스팅해 반환 */
	template<class T>
	TSharedPtr<T> GetCurrentState()
	{
		return CurrentState.IsValid() ? StaticCastSharedPtr<T>(CurrentState) : nullptr;
	}

	template<class T, typename = std::enable_if_t<std::is_enum_v<T>>>
	bool ContainsState(const T StateID) const { return StatesMap.Contains(static_cast<uint8>(StateID)); }

	bool CanTransit(uint8 TargetStateID) const;
	bool TransitState(uint8 TargetStateID);
	void UpdateState(float DeltaSeconds);

	void SetDescription(const FString& InDescription) { Description = InDescription; }
	const FString& GetDescription() const { return Description; }

	/** 상태 전이가 완료될 때마다 호출되는 델리게이트 */
	FSimpleDelegate OnStateChanged;

protected:
	TSharedPtr<FFiniteState> CurrentState;

private:
	void Init();
	TSharedPtr<FFiniteState> RegisterState(uint8 StateID, TSharedPtr<FFiniteState> State);
	void SetFirstState(uint8 StateID);
	bool SetCurrentState(uint8 StateID);
	bool TransitStateIfPending();
	bool TransitStateInternal(uint8 TargetStateID);

	FString Description;
	uint8 PrevStateID = FFiniteState::INVALID_STATE_ID;
	TMap<uint8, TSharedPtr<FFiniteState>> StatesMap;

	bool bDeferTransition = false;

	// Pending 큐: Enter/Update/Exit 도중 들어온 전이 요청을 순서대로 처리
	TQueue<uint8> PendingTransitions;
};
