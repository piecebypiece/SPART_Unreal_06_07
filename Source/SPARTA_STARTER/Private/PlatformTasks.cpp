#include "PlatformTasks.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Components/SplineComponent.h"

// FPlatformStateEvaluator
const UStruct* FPlatformStateEvaluator::GetInstanceDataType() const
{
    return FPlatformStateEvaluatorInstanceData::StaticStruct();
}

void FPlatformStateEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FPlatformStateEvaluatorInstanceData& InstanceData = Context.GetInstanceData<FPlatformStateEvaluatorInstanceData>(*this);
    APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (!Platform)
    {
        return;
    }

    InstanceData.PlatformActor = Platform;
    USplineComponent* Spline = Platform->PathSpline;
    if (!Spline || Spline->GetNumberOfSplinePoints() < 2)
    {
        return;
    }

    const float TotalSplineLength = Spline->GetSplineLength();
    InstanceData.DistanceAlongSpline = Platform->GetDistanceAlongSpline();

    // 현재 위치에 따른 포인트 인덱스와 알파 값 계산
    float Distance = InstanceData.DistanceAlongSpline;
    int32 FromIndex = 0;
    int32 ToIndex = 0;
    float Alpha = 0.0f;

    for (int32 i = 0; i < Spline->GetNumberOfSplinePoints() - 1; ++i)
    {
        float StartDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i);
        float EndDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);

        if (Distance >= StartDistance && Distance < EndDistance)
        {
            FromIndex = i;
            ToIndex = i + 1;
            Alpha = (Distance - StartDistance) / (EndDistance - StartDistance);
            break;
        }
    }
    // 마지막 포인트에 도달한 경우
    if (Distance >= Spline->GetDistanceAlongSplineAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1))
    {
        FromIndex = Spline->GetNumberOfSplinePoints() - 2;
        ToIndex = Spline->GetNumberOfSplinePoints() - 1;
        Alpha = 1.0f;
    }

    InstanceData.FromPointIndex = FromIndex;
    InstanceData.ToPointIndex = ToIndex;
    InstanceData.Alpha = Alpha;

    // 현재 위치에 보간된 속성 값 계산
    if (Platform->PointDetails.IsValidIndex(FromIndex) && Platform->PointDetails.IsValidIndex(ToIndex))
    {
        const FSplinePointDetails& FromDetails = Platform->PointDetails[FromIndex];
        const FSplinePointDetails& ToDetails = Platform->PointDetails[ToIndex];

        InstanceData.CurrentMoveSpeed = FMath::Lerp(FromDetails.Speed, ToDetails.Speed, Alpha);
        InstanceData.CurrentRotation = FMath::Lerp(FromDetails.TargetRotation, ToDetails.TargetRotation, Alpha);

        InstanceData.bShouldBeVisible = !ToDetails.bDisappear;
    }

    // bReachedPointActionTrigger 설정
    // 현재 위치가 ToPointIndex에 충분히 가까워졌고, 해당 포인트에 Action이 정의되어 있을 때
    // (Alpha가 1.0에 매우 가깝고, ToPointIndex가 유효하며, 해당 포인트에 Action이 None이 아닐 때)
    if (FMath::IsNearlyEqual(Alpha, 1.0f, 0.01f) && Platform->PointDetails.IsValidIndex(ToIndex))
    {
        const FSplinePointDetails& ToDetails = Platform->PointDetails[ToIndex];
        if (ToDetails.Action != EPlatformAction::None)
        {
            InstanceData.bReachedPointActionTrigger = true;
        }
        else
        {
            InstanceData.bReachedPointActionTrigger = false;
        }
    }
    else
    {
        InstanceData.bReachedPointActionTrigger = false;
    }

    InstanceData.bCollisionDetected = Platform->HasCollisionOccurredThisTick();
}

// FUpdateMovementTask
const UStruct* FUpdateMovementTask::GetInstanceDataType() const
{
    return FUpdateMovementTaskInstanceData::StaticStruct();
}

EStateTreeRunStatus FUpdateMovementTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FUpdateMovementTaskInstanceData& InstanceData = Context.GetInstanceData<FUpdateMovementTaskInstanceData>(*this);
    InstanceData.bReverseDirection = false;
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FUpdateMovementTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FUpdateMovementTaskInstanceData& InstanceData = Context.GetInstanceData<FUpdateMovementTaskInstanceData>(*this);
    if (!InstanceData.PlatformActor.IsValid())
    {
        return EStateTreeRunStatus::Failed;
    }

    APlatformBase* Platform = InstanceData.PlatformActor.Get();
    USplineComponent* Spline = Platform->PathSpline;
    if (!Spline)
    {
        return EStateTreeRunStatus::Failed;
    }

    float CurrentDistance = Platform->GetDistanceAlongSpline();
    const float TotalSplineLength = Spline->GetDistanceAlongSplineAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1);

    if (InstanceData.bReverseDirection)
    {
        CurrentDistance -= InstanceData.CurrentMoveSpeed * DeltaTime;
    }
    else
    {
        CurrentDistance += InstanceData.CurrentMoveSpeed * DeltaTime;
    }

    if (MovementMode == EPlatformMovementMode::Loop)
    {
        if (CurrentDistance >= TotalSplineLength)
        {
            CurrentDistance = FMath::Fmod(CurrentDistance, TotalSplineLength);
        }
        else if (CurrentDistance < 0.0f)
        {
            CurrentDistance = TotalSplineLength + FMath::Fmod(CurrentDistance, TotalSplineLength);
        }
    }
    else if (MovementMode == EPlatformMovementMode::PingPong)
    {
        if (CurrentDistance >= TotalSplineLength)
        {
            CurrentDistance = TotalSplineLength;
            InstanceData.bReverseDirection = true;
        }
        else if (CurrentDistance < 0.0f)
        {
            CurrentDistance = 0.0f;
            InstanceData.bReverseDirection = false;
        }
    }
    else // None
    {
        if (CurrentDistance >= TotalSplineLength)
        {
            CurrentDistance = TotalSplineLength;
            return EStateTreeRunStatus::Succeeded; // 이동 완료
        }
    }

    // 현재 이동 값에 맞춰서 스플라인의 위치에 맞게 Mesh 위치 설정.
    Platform->SetDistanceAlongSpline(CurrentDistance);

    //FTransform NewTransform = Spline->GetTransformAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);
    

    //FTransform ActorWorldTransform = Platform->GetActorTransform();
    //FTransform RelativeTransform = NewTransform.GetRelativeTransform(ActorWorldTransform);
    Platform->Mesh->SetRelativeLocation(NewLocation);
    
    return EStateTreeRunStatus::Running;
}


// FUpdateRotationTask
const UStruct* FUpdateRotationTask::GetInstanceDataType() const
{
    return FUpdateRotationTaskInstanceData::StaticStruct();
}

EStateTreeRunStatus FUpdateRotationTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FUpdateRotationTaskInstanceData& InstanceData = Context.GetInstanceData<FUpdateRotationTaskInstanceData>(*this);
    if (InstanceData.PlatformActor.IsValid())
    { // 목표 회전까지 보간 회전
        FRotator CurrentRelativeRotation = InstanceData.PlatformActor->Mesh->GetRelativeRotation();
        FRotator NewRelativeRotation = FMath::Lerp(CurrentRelativeRotation, InstanceData.CurrentRotation, DeltaTime * 5.0f); // 5.0f는 보간 속도, 조절 가능
        InstanceData.PlatformActor->Mesh->SetRelativeRotation(NewRelativeRotation);
    }
    return EStateTreeRunStatus::Running;
}


// FUpdateVisibilityTask
const UStruct* FUpdateVisibilityTask::GetInstanceDataType() const
{
    return FUpdateVisibilityTaskInstanceData::StaticStruct();
}

EStateTreeRunStatus FUpdateVisibilityTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FUpdateVisibilityTaskInstanceData& InstanceData = Context.GetInstanceData<FUpdateVisibilityTaskInstanceData>(*this);
    if (InstanceData.PlatformActor.IsValid())
    {   
        bool bIsCurrentlyVisible = !InstanceData.PlatformActor->IsHidden();
        if (bIsCurrentlyVisible != InstanceData.bShouldBeVisible)
        {
            InstanceData.PlatformActor->SetActorHiddenInGame(!InstanceData.bShouldBeVisible);
            InstanceData.PlatformActor->SetActorEnableCollision(InstanceData.bShouldBeVisible);
        }
    }
    return EStateTreeRunStatus::Running;
}

// FPauseTask
const UStruct* FPauseTask::GetInstanceDataType() const
{
    return FPauseTaskInstanceData::StaticStruct();
}

EStateTreeRunStatus FPauseTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FPauseTaskInstanceData& InstanceData = Context.GetInstanceData<FPauseTaskInstanceData>(*this);
    InstanceData.ElapsedTime = 0.0f;
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FPauseTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FPauseTaskInstanceData& InstanceData = Context.GetInstanceData<FPauseTaskInstanceData>(*this);
    InstanceData.ElapsedTime += DeltaTime;

    if (InstanceData.ElapsedTime >= Duration)
    {
        return EStateTreeRunStatus::Succeeded;
    }
    return EStateTreeRunStatus::Running;
}