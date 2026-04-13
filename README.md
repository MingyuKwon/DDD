# DDD

Unreal Engine 5.7 기반 3인칭 스택 전투 게임.
플레이어가 버프 총알을 적에게 누적시키고 연산 결과로 전투하는 메카닉.

---

## 빠른 시작

```bash
git clone https://github.com/MingyuKwon/DDD.git
```

1. `DDD.uproject` 우클릭 → **Generate Visual Studio project files**
2. `DDD.sln` 열기
3. 구성: `DebugGame_Editor | x64` → **Ctrl+Shift+B** 빌드
4. `DDD.uproject` 로 에디터 열기

> 클론 직후 빌드가 필수입니다 — `/Binaries`, `/Intermediate`는 git에서 제외되어 있습니다.

---

## 문서

| 문서 | 내용 |
|---|---|
| [환경 설정 및 빌드](doc/setup.md) | 요구 사항, 엔진 경로 동작 방식, 빌드 구성 |
| [코드 구조](doc/architecture.md) | 소스 폴더 구조, 클래스 레퍼런스, C++↔Blueprint 패턴 |
| [게임플레이 시스템](doc/systems.md) | 버프 스택, 인벤토리, 락온, UI 계층 |
| [트러블슈팅](doc/troubleshooting.md) | 자주 발생하는 문제와 해결법 |

---

## 핵심 기술 스택

- **엔진**: Unreal Engine 5.7
- **언어**: C++ + Blueprint
- **렌더링**: Lumen, Ray Tracing, DX12 SM6
- **입력**: Enhanced Input System
