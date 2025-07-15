#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "StateTreeTaskBase.h"
#include "PlatformBase.h"
#include "PlatformTasks.generated.h"

// 전방 선언
class APlatformBase;

/**
 * PlatformBase의 상태를 평가하여 StateTree의 다른 Task들이 사용할 수 있도록 데이터를 제공하는 Evaluator입니다.
 */
USTRUCT()
struct SPARTA_STARTER_API FPlatformStateEvaluator : public FStateTreeEvaluatorCommonBase
{
    GENERATED_BODY()

    virtual const UStruct* GetInstanceDataType() const override;
    virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

    // UPROPERTY()
    // TStateTreeExternalDataHandle<APlatformBase> PlatformActorHandle;
};

USTRUCT()
struct SPARTA_STARTER_API FPlatformStateEvaluatorInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Output")
    TWeakObjectPtr<APlatformBase> PlatformActor;

    UPROPERTY(EditAnywhere, Category = "Output")
    float CurrentMoveSpeed = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Output")
    FRotator CurrentRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Category = "Output")
    bool bShouldBeVisible = true;

    UPROPERTY(EditAnywhere, Category = "Output")
    float DistanceAlongSpline = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Output")
    int32 FromPointIndex = 0;

    UPROPERTY(EditAnywhere, Category = "Output")
    int32 ToPointIndex = 0;

    UPROPERTY(EditAnywhere, Category = "Output")
    float Alpha = 0.0f;
};


/**
 * 플랫폼의 이동을 담당하는 Task입니다.
 */
USTRUCT()
struct SPARTA_STARTER_API FUpdateMovementTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    virtual const UStruct* GetInstanceDataType() const override;
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

    UPROPERTY(EditAnywhere, Category = "Settings")
    bool bLoop = false;
};

USTRUCT()
struct SPARTA_STARTER_API FUpdateMovementTaskInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
    TWeakObjectPtr<APlatformBase> PlatformActor;

    UPROPERTY(EditAnywhere, Category = "Input")
    float CurrentMoveSpeed = 0.0f;
};

/**
 * 플랫폼의 추가 회전을 담당하는 Task입니다.
 */
USTRUCT()
struct SPARTA_STARTER_API FUpdateRotationTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    virtual const UStruct* GetInstanceDataType() const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT()
struct SPARTA_STARTER_API FUpdateRotationTaskInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
    TWeakObjectPtr<APlatformBase> PlatformActor;

    UPROPERTY(EditAnywhere, Category = "Input")
    FRotator CurrentRotation = FRotator::ZeroRotator;
};


/**
 * 플랫폼의 가시성을 담당하는 Task입니다.
 */
USTRUCT()
struct SPARTA_STARTER_API FUpdateVisibilityTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    virtual const UStruct* GetInstanceDataType() const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT()
struct SPARTA_STARTER_API FUpdateVisibilityTaskInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
    TWeakObjectPtr<APlatformBase> PlatformActor;

    UPROPERTY(EditAnywhere, Category = "Input")
    bool bShouldBeVisible = true;
};