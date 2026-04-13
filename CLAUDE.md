# CLAUDE.md

UE 5.7 3인칭 스택 전투 게임. C++ 컴포넌트 + Blueprint 구현체 혼합 구조.

## 문서 인덱스

| 문서 | 내용 |
|---|---|
| [doc/setup.md](doc/setup.md) | 환경, 엔진 경로, 빌드 방법, Target.cs 설정 |
| [doc/architecture.md](doc/architecture.md) | 소스 구조, include 규칙, 전체 클래스 레퍼런스 |
| [doc/systems.md](doc/systems.md) | 버프 스택, 인벤토리, 락온, UI 계층 |
| [doc/troubleshooting.md](doc/troubleshooting.md) | 빌드 에러, IntelliSense 문제 해결법 |

---

## 즉시 참고할 것들

**빌드 구성**: `DebugGame_Editor | x64`

**엔진 경로**: 하드코딩 없음. `DDD.uproject`의 `"EngineAssociation": "5.7"` → 레지스트리 자동 조회.

**새 PC 클론 후**: 빌드 1회 필수 (`/Binaries`, `/Intermediate` gitignore 제외)

**Target.cs**: `BuildSettingsVersion.V6` + `EngineIncludeOrderVersion.Unreal5_7` (V5는 UE 5.7과 충돌)

**include 패턴** (`Source/DDD/`가 include root):
```cpp
#include "DDDComponent/HealthManagerComponent.h"  // 폴더 prefix + 파일명
#include "Character/DDDCharacter.h"
#include "DDD.h"                                   // 루트 파일은 파일명만
```
`"DDD/DDD.h"` 형태는 사용하지 않습니다.

**BlueprintImplementableEvent 주의**:
`bridgeCallLockOnOff`는 C++에서 구현하면 안 됨 — `BPC_BattleUI` Blueprint에서만 구현.

**모듈 의존성 추가**: `Source/DDD/DDD.Build.cs` 의 `PublicDependencyModuleNames`

---

## C++ ↔ Blueprint 매핑

| C++ 클래스 | Blueprint 구현체 |
|---|---|
| `UBattleUIComponent` | `Content/Custom/Blueprint/Component/BPC_BattleUI` |
| `UEnemyUIComponent` | `BPC_EnemyUI` |
| `UInventoryComponent` | `BPC_Inventory` |
| `ADDDGameMode` | `Content/Custom/GameMode/BP_DDDGameplay` |

---

## 핵심 타입 (DDD.h)

```cpp
enum class EBuffType : uint8 { ENone, EPlus, EMultiple, EMinus, EDivide, EMAX };
struct FBuff          { EBuffType BuffType; int32 Operand; };
struct FBulletInfo    { EBuffType BuffType; int32 bulletRangeMin, bulletRangeMax, cost; };
```
