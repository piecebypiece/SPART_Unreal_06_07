#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionTypes.h"
#include "StateTreeLinker.h"
#include "PlatformBase.h"
#include "PlatformTasks.generated.h"

USTRUCT()
struct SPARTA_STARTER_API FStateTreeSplinePointCountEvaluatorInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Output")
	int32 PointCount = 0;
};

USTRUCT()
struct SPARTA_STARTER_API FStateTreeSplinePointCountEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	virtual const UStruct* GetInstanceDataType() const override
	{
		UE_LOG(LogTemp, Warning, TEXT("FStateTreeSplinePointCountEvaluator::GetInstanceDataType called!"));
		return FStateTreeSplinePointCountEvaluatorInstanceData::StaticStruct();
	}
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	//virtual bool Link(FStateTreeLinker& Linker) override;

protected:
	// 외부의 데이터를 안전하게 접근하려면 사용해야 하는데 현재로서 방법이 명확치 않음으로 Actor 접근으로 대체
	//TStateTreeExternalDataHandle<APlatformBase> PlatformActorHandle;
};

USTRUCT()
struct SPARTA_STARTER_API FStateTreeFindNextPointTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "InputOutput")
	int32 TargetPointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "InputOutput")
	bool IsReversing = false;

	UPROPERTY(EditAnywhere, Category = "Input")
	int32 PointCount = 0;
};

USTRUCT()
struct SPARTA_STARTER_API FStateTreeFindNextPointTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FStateTreeFindNextPointTask();

	virtual const UStruct* GetInstanceDataType() const override
	{
		UE_LOG(LogTemp, Warning, TEXT("FStateTreeFindNextPointTask::GetInstanceDataType called!"));
		return FStateTreeFindNextPointTaskInstanceData::StaticStruct();
	}
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
protected:

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bLoop = false;
};
