# StateTree 기반 동적 플랫폼 구현 계획 (C++ 중심)

이 문서는 언리얼 엔진의 `StateTree`를 활용하여 플랫포밍 게임의 동적 플랫폼 시스템을 구축하는 계획을 C++ 중심으로 정리합니다.

## 1. 핵심 아키텍처: 역할 분리

- **`APlatformBase` (C++ 클래스):** 모든 플랫폼의 부모 클래스. 외형, 데이터, 기본 컴포넌트를 C++ 코드로 정의. 블루프린트는 이 클래스를 상속받아 외형(메시)과 데이터 값만 설정하는 용도로 사용.
- **`StateTree` 에셋:** 플랫폼의 행동 로직을 시각적으로 설계하는 데이터 에셋. C++로 작성된 Task와 Evaluator를 조합하여 사용.
- **`APuzzleGenerator` (C++ 클래스):** 게임 시작 시 `APlatformBase`를 동적으로 스폰하고, 무작위 `StateTree`와 속성을 C++ 코드로 부여하여 랜덤 퍼즐을 생성.

---

## 2. `APlatformBase`: C++ 데이터 컨테이너

*   **헤더 파일 (`PlatformBase.h`):**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

class UStateTree; // Forward Declaration
class UStateTreeComponent;
class UStaticMeshComponent;

UCLASS()
class YOURPROJECT_API APlatformBase : public AActor
{
    GENERATED_BODY()

public:
    APlatformBase();

protected:
    virtual void BeginPlay() override;

public:
    // 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStateTreeComponent* StateTreeComponent;

    // 리플렉션 적용 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Logic")
    TSoftObjectPtr<UStateTree> StateTreeAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Properties|Movement")
    float MovementSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Properties|Movement")
    float MovementRange = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Properties|Rotation")
    FRotator RotationSpeed = FRotator(0, 90.0f, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Properties|Timer")
    float WaitTime = 2.0f;

    // 내부 상태 변수
    FVector StartLocation;
    FVector MoveDirection = FVector::ForwardVector;
};
```

---

## 3. `StateTree`: C++ 기반 행동 부품

### 재사용 가능한 C++ 빌딩 블록 (Tasks & Evaluators)

- **Evaluators (상황 판단 센서):** `FStateTreeEvaluatorBase`를 상속받는 구조체로 작성.
    - `FDistanceCheckEvaluator`: `StartLocation`과 현재 위치를 비교하여 `MovementRange` 초과 여부 판단.

- **Tasks (실제 행동):** `FStateTreeTaskBase`를 상속받는 구조체로 작성.
    - `FMoveTask`: `Tick()` 함수 내에서 `DeltaTime`을 활용하여 프레임 독립적인 이동 처리.
    - `FRotateTask`: `Tick()` 함수 내에서 `DeltaTime`을 활용하여 프레임 독립적인 회전 처리.
    - `FWaitWithTimerTask`: `EnterState()`에서 `GetWorld()->GetTimerManager().SetTimer()`를 호출하고, 완료 시 `StateTree`에 `Gameplay Event`를 전송.
    - `FToggleVisibilityTask`: 플랫폼의 가시성과 충돌을 C++ 코드로 제어.

### StateTree 에셋
- C++로 작성된 Task와 Evaluator들을 `StateTree Editor`에서 시각적으로 조합하여 `ST_PatrollingPlatform`, `ST_DisappearingPlatform` 등의 행동 로직을 완성.

---

## 4. `APuzzleGenerator`: C++ 랜덤 퍼즐 생성

*   **헤더 파일 (`PuzzleGenerator.h`):**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleGenerator.generated.h"

class UStateTree;

UCLASS()
class YOURPROJECT_API APuzzleGenerator : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Puzzle Generation")
    TSubclassOf<class APlatformBase> PlatformClass;

    UPROPERTY(EditAnywhere, Category = "Puzzle Generation")
    TArray<TSoftObjectPtr<UStateTree>> PlatformBehaviorPool;

    UPROPERTY(EditAnywhere, Category = "Puzzle Generation")
    int32 NumberOfPlatformsToSpawn = 10;

    UPROPERTY(EditAnywhere, Category = "Puzzle Generation")
    FBox SpawnVolume;
};
```

*   **소스 파일 (`PuzzleGenerator.cpp`):** `BeginPlay()` 함수 내에서 루프를 돌며 `FMath::RandPointInBox()`, `FMath::RandRange()` 등을 사용하여 속성을 랜덤화하고, `GetWorld()->SpawnActor<APlatformBase>()`를 통해 플랫폼을 동적으로 생성 및 설정.

---

## 5. 과제 체크리스트 충족 방안 (C++ 기준)

- **회전/이동 기능:** C++ `FMoveTask`, `FRotateTask`가 `DeltaTime`을 사용하여 프레임 독립성을 보장.
- **리플렉션:** `APlatformBase` 헤더의 `UPROPERTY` 매크로가 에디터에서의 즉시 수정을 완벽하게 지원.
- **타이머 시스템:** `FWaitWithTimerTask`가 C++ 네이티브 `GetWorld()->GetTimerManager()`를 사용하여 도전 과제 1번을 충족.
- **랜덤 퍼즐 생성:** `APuzzleGenerator`가 C++ 코드로 동적 스폰과 속성 랜덤화를 구현하여 도전 과제 2번을 충족.