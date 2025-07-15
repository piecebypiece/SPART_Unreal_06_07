#include "PlatformTasks.h"
#include "PlatformBase.h"
#include "Components/SplineComponent.h"
#include "StateTreeExecutionContext.h"

//bool FStateTreeSplinePointCountEvaluator::Link(FStateTreeLinker& Linker)
//{
//	Linker.LinkExternalData(PlatformActorHandle);
//	return true;
//}

void FStateTreeSplinePointCountEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const APlatformBase* PlatformActor = Cast<APlatformBase>(Context.GetOwner());
	FStateTreeSplinePointCountEvaluatorInstanceData& InstanceData = Context.GetInstanceData<FStateTreeSplinePointCountEvaluatorInstanceData>(*this);

	if (!PlatformActor || !PlatformActor->PathSpline)
	{
		InstanceData.PointCount = 0;
		return;
	}

	InstanceData.PointCount = PlatformActor->PathSpline->GetNumberOfSplinePoints();
}

FStateTreeFindNextPointTask::FStateTreeFindNextPointTask()
{
	bShouldCallTick = false;
}

EStateTreeRunStatus FStateTreeFindNextPointTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	APlatformBase* PlatformActor = Cast<APlatformBase>(Context.GetOwner());
	FStateTreeFindNextPointTaskInstanceData& InstanceData = Context.GetInstanceData<FStateTreeFindNextPointTaskInstanceData>(*this);

	const int32 PointCount = InstanceData.PointCount;

	if (!PlatformActor || PointCount < 2)
	{
		return EStateTreeRunStatus::Failed;
	}

	int32 NextPointIndex = PlatformActor->CurrentTargetPointIndex;
	bool bCurrentIsReversing = PlatformActor->bIsReversing;

	if (bCurrentIsReversing)
	{
		NextPointIndex--;
		if (NextPointIndex < 0)
		{
			if (bLoop)
			{
				NextPointIndex = PointCount - 1;
			}
			else
			{
				NextPointIndex = 1;
				bCurrentIsReversing = false;
			}
		}
	}
	else
	{
		NextPointIndex++;
		if (NextPointIndex >= PointCount)
		{
			if (bLoop)
			{
				NextPointIndex = 0;
			}
			else
			{
				NextPointIndex = PointCount - 2;
				bCurrentIsReversing = true;
			}
		}
	}

	PlatformActor->CurrentTargetPointIndex = NextPointIndex;
	PlatformActor->bIsReversing = bCurrentIsReversing;

	return EStateTreeRunStatus::Succeeded;
}