#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeEvaluatorBase.h"
#include "PlatformTasks.generated.h"

USTRUCT()
struct SPARTA_STARTER_API FEmptyTaskInstance
{
    GENERATED_BODY()
};

USTRUCT()
struct SPARTA_STARTER_API FPlatformTaskBase : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

public:
    virtual const UStruct* GetInstanceDataType() const override { return FEmptyTaskInstance::StaticStruct(); }
};
        
USTRUCT(meta = (DisplayName = "MoveTask", Category = "Platform"))
struct SPARTA_STARTER_API FMoveTask : public FPlatformTaskBase
{
    GENERATED_BODY()

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
    
};

USTRUCT(meta = (DisplayName = "RotateTask", Category = "Platform"))
struct SPARTA_STARTER_API FRotateTask : public FPlatformTaskBase
{
    GENERATED_BODY()

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};

USTRUCT(meta = (DisplayName = "WaitWithTimerTask", Category = "Platform"))
struct SPARTA_STARTER_API FWaitWithTimerTask : public FPlatformTaskBase
{
    GENERATED_BODY()

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(meta = (DisplayName = "ToggleVisibilityTask", Category = "Platform"))
struct SPARTA_STARTER_API FToggleVisibilityTask : public FPlatformTaskBase
{
    GENERATED_BODY()

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

USTRUCT(meta = (DisplayName = "DistanceCheckEvaluator"))
struct SPARTA_STARTER_API FDistanceCheckEvaluator : public FStateTreeEvaluatorBase
{
    GENERATED_BODY()

    virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
