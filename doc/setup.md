# 환경 설정 및 빌드

## 요구 사항

| 항목 | 내용 |
|---|---|
| Unreal Engine | 5.7 (Epic Games Launcher로 설치) |
| Visual Studio | 2022 v17.7 이상 |
| VS 워크로드 | Game development with C++ |
| VS 컴포넌트 | `Component.Unreal.Ide`, `Component.Unreal.Debugger` |

## 엔진 경로에 대해

엔진 경로는 프로젝트에 **하드코딩되어 있지 않습니다.**

`DDD.uproject` 의 `"EngineAssociation": "5.7"` 값을 기반으로 UBT가 아래 레지스트리 키를 조회해 경로를 동적으로 찾습니다.

```
HKLM\SOFTWARE\EpicGames\Unreal Engine\5.7\InstalledDirectory
```

따라서 UE 5.7이 어느 드라이브/경로에 설치되어 있어도 정상 작동합니다.

> `Intermediate/ProjectFiles/UECommon.props` 파일 안에 절대 경로가 보이지만, 이 파일은 `.gitignore`로 제외된 자동생성 파일이므로 직접 수정할 필요가 없습니다.

## 처음 시작하기

```bash
# 1. 저장소 클론
git clone https://github.com/MingyuKwon/DDD.git
cd DDD_Origin

# 2. 프로젝트 파일 생성
#    DDD.uproject 우클릭 → "Generate Visual Studio project files"

# 3. Visual Studio에서 솔루션 열기
#    DDD.sln 더블클릭

# 4. 빌드 구성 설정
#    상단 드롭다운: DebugGame_Editor | x64

# 5. 최초 빌드 실행 (필수)
#    Ctrl+Shift+B  또는  DDD 프로젝트 우클릭 → Build
```

> **클론 직후 빌드가 반드시 필요한 이유**
> `.gitignore`로 `/Binaries`, `/Intermediate`, `/Saved` 가 제외되어 있습니다.
> 빌드를 해야 UHT가 `.generated.h` 파일을 생성하고 IntelliSense가 활성화됩니다.

## 빌드 구성

| 구성 | 용도 |
|---|---|
| `DebugGame_Editor \| x64` | 일반 개발 (에디터 + 디버깅) |
| `Development_Editor \| x64` | 최적화된 에디터 빌드 |
| `DebugGame \| x64` | 독립 실행 디버그 빌드 |

## Target.cs 설정

`DDD.Target.cs` / `DDDEditor.Target.cs` 는 반드시 아래 버전을 사용해야 합니다.

```csharp
DefaultBuildSettings = BuildSettingsVersion.V6;
IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
```

V5 이하는 UE 5.7 기본값과 충돌해 빌드 실패합니다.
