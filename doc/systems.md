# 게임플레이 시스템

## 버프 스택 시스템

플레이어가 총알을 사용할 때 적에게 버프를 쌓고, 쌓인 스택의 연산 결과로 피해를 계산합니다.

### 버프 타입 (`EBuffType`)

| 값 | 연산 | 설명 |
|---|---|---|
| `ENone` | 없음 | 기본값 |
| `EPlus` | `l + r` | 덧셈 |
| `EMinus` | `l - r` | 뺄셈 |
| `EMultiple` | `l * r` | 곱셈 |
| `EDivide` | `l / r` | 나눗셈 (0 나누기 방지) |

### 연산 흐름

```
1. 플레이어가 인벤토리에서 총알(FBulletInfo) 선택
2. 적의 UHealthManagerComponent.buffStack 에 FBuff 추가
3. UDDDFunctionLibrary::CalcResultInt32() 로 스택 연산 처리
4. 결과값이 체력에 적용
```

### 관련 구조체

```cpp
// 스택에 쌓이는 단위
struct FBuff {
    EBuffType BuffType;  // 어떤 연산인가
    int32 Operand;       // 피연산자
};

// 총알 1개의 정보
struct FBulletInfo {
    EBuffType BuffType;
    int32 bulletRangeMin;  // 피연산자 최솟값 (랜덤 범위)
    int32 bulletRangeMax;  // 피연산자 최댓값
    int32 cost;            // 사용 비용
};
```

---

## 인벤토리 시스템

`UInventoryComponent`가 두 종류의 총알 슬롯을 관리합니다.

| 배열 | 크기 | 설명 |
|---|---|---|
| `EquipBullets` | 6 | 플레이어가 직접 장착한 슬롯 |
| `RandomBullets` | 6 | 랜덤으로 채워지는 슬롯 |

두 배열 모두 생성자에서 6개의 기본값으로 초기화됩니다.

---

## 락온(Lock-On) 시스템

`UBattleUIComponent`가 전투 중 적 대상 선택을 담당합니다.

### 적 등록

```cpp
// positionIndex 슬롯에 적을 등록
// 이미 등록됐거나 슬롯 범위 초과 시 false 반환
bool registerEnemy(int32 positionIndex, AActor* enemyActor,
                   TArray<AActor*>& enemyRegisterArray);
```

### 락온 대상 전환

```cpp
// defaultUnit: 0=현재 유지, +1=다음 적, -1=이전 적
void FindEnemyToLock(int32& positionIndex, int defaultUnit,
                     const TArray<AActor*>& enemyRegisterArray);
```

내부 동작:
1. `positionIndex`를 `defaultUnit`만큼 이동
2. 유효한(null이 아닌) 적을 찾을 때까지 순방향/역방향 탐색
3. 찾으면 해당 인덱스의 적을 락온, 나머지는 락오프
4. 못 찾으면 인덱스를 경계값으로 클램프

### UI 연동

```cpp
// Blueprint에서 구현 (BPC_BattleUI)
void bridgeCallLockOnOff(bool bLockOn, AActor* enemyActor);
```

`FindEnemyToLock`이 각 적 Actor에 대해 이 이벤트를 호출합니다.
Blueprint에서 `bLockOn` 값에 따라 락온 위젯을 표시/숨깁니다.

---

## UI 시스템

### 인게임 HUD 계층

```
WBP_BattleUI (메인 전투 HUD)
├── WBP_HealthBar       — 플레이어 체력바
├── WBP_Target          — 락온 대상 마커
├── WBP_BuffStack       — 버프 스택 시각화
│   └── WBP_BuffStackBucket  — 버프 1칸
├── WBP_SkillInfroPanel — 스킬 정보 패널
└── WBP_StackHealth     — 스택 기반 체력 표시
```

### 메인 메뉴

```
WBP_MainMenuHUD
├── WBP_SlidingMenu     — 슬라이딩 메뉴
├── WBP_NewGame         — 새 게임 화면
├── WBP_Tutorial        — 튜토리얼
├── WBP_Note / WBP_Alert / WBP_ClickNote
└── Setting/            — 설정 화면
```

---

## 적 시스템

- 적 종류: Sword Robot (`Content/EnemyAsset/Sword_Robot/`)
- 애니메이션 블루프린트: `ABP_SwordEnemy`, `ABPT_Enemy` (템플릿)
- 적 컴포넌트: `BPC_EnemyUI` (EnemyUIComponent 구현), `BPC_EnemyHealth`

---

## 렌더링 설정

```
Lumen (동적 GI)     r.DynamicGlobalIlluminationMethod=1
Ray Tracing         r.RayTracing=True
Virtual Shadow Maps r.Shadow.Virtual.Enable=1
DX12 / SM6          DefaultGraphicsRHI_DX12
Static Lighting     r.AllowStaticLighting=False
```
