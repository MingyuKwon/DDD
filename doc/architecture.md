# 코드 구조 및 아키텍처

## 소스 폴더 구조

`Public/Private` 구조 없이 기능 단위 폴더로 구성됩니다. `.h`와 `.cpp`가 같은 폴더에 있습니다.

```
Source/
├── DDD.Target.cs               # 게임 빌드 타겟
├── DDDEditor.Target.cs         # 에디터 빌드 타겟
└── DDD/
    ├── DDD.Build.cs            # 모듈 빌드 규칙
    ├── DDD.h / DDD.cpp         # 전역 열거형 + 모듈 구현
    ├── DDDFunctionLibrary.h / .cpp
    ├── DDDLog.h
    ├── StringFunctionLibrary.h
    ├── Character/
    │   ├── DDDCharacter.h
    │   └── DDDCharacter.cpp
    ├── GameMode/
    │   ├── DDDGameMode.h
    │   └── DDDGameMode.cpp
    ├── DDDComponent/
    │   ├── BattleUIComponent.h / .cpp
    │   ├── EnemyUIComponent.h / .cpp
    │   ├── HealthManagerComponent.h / .cpp
    │   └── InventoryComponent.h / .cpp
    ├── AbilitySystem/
    │   ├── DDDAbilitySystemComponent.h / .cpp
    │   ├── DDDAttributeSet.h / .cpp
    │   ├── DDDGameplayTags.h / .cpp
    │   └── Ability/
    │       ├── DDDGameplayAbility.h / .cpp
    │       └── BulletShotAbility.h / .cpp
    └── DataStruct/
        ├── FiniteState.h / .cpp
        └── FiniteStateMachine.h / .cpp
```

## include 경로 규칙

`DDD.Build.cs`에 `PublicIncludePaths.Add(ModuleDirectory)`가 설정되어 있어 `Source/DDD/`가 include root입니다.

```cpp
// 폴더 경로를 prefix로 사용 — Source/DDD/ 기준
#include "DDDComponent/HealthManagerComponent.h"
#include "Character/DDDCharacter.h"
#include "AbilitySystem/DDDAbilitySystemComponent.h"

// 모듈 루트(Source/DDD/)에 있는 파일은 파일명만 사용
#include "DDD.h"
#include "DDDFunctionLibrary.h"

// 같은 폴더 안의 파일도 파일명만 사용 가능 (DataStruct/ 등)
#include "FiniteState.h"
```

> **주의**: `"DDD/DDD.h"` 형태는 사용하지 않습니다. `Source/DDD/`가 include root이므로 `"DDD.h"`로 충분합니다.

## 모듈 의존성

`DDD.Build.cs`:
```csharp
PublicIncludePaths.Add(ModuleDirectory);  // Source/DDD/ 를 include root로 등록

PublicDependencyModuleNames.AddRange(new string[]
{
    "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
    "GameplayAbilities", "GameplayTags", "GameplayTasks"
});
```

새 모듈 추가 예시:
```csharp
// UI 관련
PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
// 물리
PublicDependencyModuleNames.Add("Chaos");
```

---

## 클래스 레퍼런스

### `DDD.h` — 전역 타입

```cpp
UENUM(BlueprintType)
enum class EBuffType : uint8
{
    ENone, EPlus, EMultiple, EMinus, EDivide, EMAX
};
```

---

### `ADDDCharacter`

`ACharacter` 상속. Enhanced Input 사용.

| 멤버 | 설명 |
|---|---|
| `CameraBoom` | SpringArm, TargetArmLength=400 |
| `FollowCamera` | 3인칭 카메라 |
| `DefaultMappingContext` | Enhanced Input Mapping |
| `JumpAction / MoveAction / LookAction` | 입력 액션 |

---

### `UHealthManagerComponent`

체력과 버프 스택을 관리하는 컴포넌트.

```cpp
// 프로퍼티
int32 currentHealth;
int32 maxHealth;
TArray<FBuff> buffStack;  // 누적된 버프 목록

// FBuff 구조체
struct FBuff {
    EBuffType BuffType;
    int32 Operand;
};
```

---

### `UBattleUIComponent`

적 등록 및 락온 대상 선택을 담당.

```cpp
// 적을 특정 슬롯에 등록. 이미 등록됐거나 범위 초과 시 false 반환
bool registerEnemy(int32 positionIndex, AActor* enemyActor,
                   TArray<AActor*>& enemyRegisterArray);

// 락온 대상 변경
// defaultUnit:  0 = 현재 유지,  +1 = 다음,  -1 = 이전
void FindEnemyToLock(int32& positionIndex, int defaultUnit,
                     const TArray<AActor*>& enemyRegisterArray);

// Blueprint에서 구현 — 락온 UI 표시/해제
UFUNCTION(BlueprintImplementableEvent)
void bridgeCallLockOnOff(bool bLockOn, AActor* enemyActor);
```

> `bridgeCallLockOnOff`는 `BlueprintImplementableEvent`이므로 C++에서 구현하면 안 됩니다. `BPC_BattleUI` Blueprint에서 구현합니다.

---

### `UInventoryComponent`

총알 슬롯 관리.

```cpp
TArray<FBulletInfo> EquipBullets;   // 장착 슬롯 6개
TArray<FBulletInfo> RandomBullets;  // 랜덤 슬롯 6개

// FBulletInfo 구조체
struct FBulletInfo {
    EBuffType BuffType;
    int32 bulletRangeMin;
    int32 bulletRangeMax;
    int32 cost;
};
```

생성자에서 두 배열 모두 6개의 기본 `FBulletInfo`로 초기화됩니다.

---

### `UDDDFunctionLibrary`

Blueprint에서 호출 가능한 정적 유틸 함수 모음.

```cpp
// EBuffType에 따라 정수 연산 수행
// EPlus: l+r, EMinus: l-r, EMultiple: l*r, EDivide: l/r (0 나누기 방지)
UFUNCTION(BlueprintCallable, Category = "DDD Library")
static int32 CalcResultInt32(int32 leftOperand, int32 rightOperand,
                              EBuffType buffType);
```

---

## C++ ↔ Blueprint 연동 패턴

이 프로젝트는 C++로 인터페이스/로직을 정의하고 Blueprint로 구현/확장하는 패턴을 사용합니다.

| C++ 클래스 | Blueprint 구현체 |
|---|---|
| `UBattleUIComponent` | `BPC_BattleUI` |
| `UEnemyUIComponent` | `BPC_EnemyUI` |
| `UHealthManagerComponent` | (직접 사용) |
| `UInventoryComponent` | `BPC_Inventory` |

- `UFUNCTION(BlueprintImplementableEvent)` → C++에서 호출, Blueprint에서 구현
- `UFUNCTION(BlueprintCallable)` → Blueprint에서 호출 가능
- `UPROPERTY(BlueprintReadWrite)` → Blueprint에서 읽기/쓰기 가능
