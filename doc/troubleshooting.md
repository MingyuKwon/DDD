# 트러블슈팅

## IntelliSense가 헤더를 못 찾음 (빨간 줄)

**원인**: UE 5.3+ 에서 vcxproj에 `NMakeIncludeSearchPath`를 더 이상 삽입하지 않음.

**해결 순서**:
1. VS 2022에 `Component.Unreal.Ide` 컴포넌트가 설치되어 있는지 확인
   - Visual Studio Installer → 수정 → 개별 구성 요소 탭 → "Unreal" 검색
2. 프로젝트를 **최소 1회 빌드** (`DebugGame_Editor | x64`, Ctrl+Shift+B)
   - 빌드 후 `Intermediate/Build/` 에 `.generated.h` 파일 생성됨
   - 이후 IntelliSense 자동 활성화
3. 그래도 안 되면 VS 재시작 또는 `.vs/` 폴더 삭제 후 재오픈

---

## 빌드 실패: BuildSettingsVersion 충돌

```
DDDEditor modifies the values of properties: [ UndefinedIdentifierWarningLevel: Off != Error ]
```

**원인**: `Target.cs`의 `BuildSettingsVersion.V5`가 UE 5.7 기본값(V6)과 충돌.

**해결**: `DDD.Target.cs` 및 `DDDEditor.Target.cs` 수정

```csharp
DefaultBuildSettings = BuildSettingsVersion.V6;      // V5 → V6
IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;  // 5_5 → 5_7
```

---

## Generate Visual Studio project files 후에도 IntelliSense 안 됨

프로젝트 파일 재생성은 IntelliSense 경로 문제를 해결하지 못합니다.
빌드를 실행해야 합니다 (위 항목 참조).

---

## 새 PC에서 클론 후 에디터가 안 열림

`.gitignore`로 `/Binaries`, `/Intermediate`가 제외되어 있어서 모듈 DLL이 없습니다.

```
1. DDD.uproject 우클릭 → Generate Visual Studio project files
2. DDD.sln 열기
3. DebugGame_Editor | x64 구성으로 빌드
4. 빌드 성공 후 DDD.uproject로 에디터 열기
```

---

## 엔진을 다른 경로에 재설치했을 때

프로젝트 파일에는 엔진 경로가 하드코딩되어 있지 않습니다.
단, `Intermediate/ProjectFiles/` 안의 `UECommon.props`는 이전 경로를 가리킬 수 있습니다.

**해결**: `Intermediate/ProjectFiles/` 폴더를 삭제하고 "Generate Visual Studio project files" 재실행.

---

## 새 C++ 클래스 추가 후 에디터 크래시

UCLASS/USTRUCT를 추가한 뒤 에디터를 열기 전에 반드시 빌드해야 합니다.
UHT가 `.generated.h`를 먼저 만들어야 에디터가 로드할 수 있습니다.

---

## Blueprint 컴파일 에러 (부모 클래스 변경 후)

C++ 부모 클래스의 함수/프로퍼티 시그니처를 변경하면 자식 Blueprint도 재컴파일이 필요합니다.
에디터에서 해당 Blueprint를 열고 **Compile** 버튼을 클릭합니다.
