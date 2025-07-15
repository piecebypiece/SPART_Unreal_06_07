#include "PlatformTasks.h"
#include "PlatformBase.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Actor.h"

//void FDistanceCheckEvaluator::Evaluate(FStateTreeExecutionContext& Context, const EStateTreeEvaluationType EvalType, const float DeltaTime) const
//{
//    const APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
//    if (Platform)
//    {
//        const float Distance = FVector::Dist(Platform->StartLocation, Platform->GetActorLocation());
//        // This is a placeholder. The actual logic to signal the StateTree will be more complex.
//        // For now, we'll just log it.
//        UE_LOG(LogTemp, Warning, TEXT("Distance: %f / %f"), Distance, Platform->MovementRange);
//    }
//}

void FDistanceCheckEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    const APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (Platform)
    {
        const float Distance = FVector::Dist(Platform->StartLocation, Platform->GetActorLocation());
        // This is a placeholder. The actual logic to signal the StateTree will be more complex.
        // For now, we'll just log it.
        UE_LOG(LogTemp, Warning, TEXT("Distance: %f / %f"), Distance, Platform->MovementRange);
    }
}

EStateTreeRunStatus FMoveTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (Platform)
    {
        FVector NewLocation = Platform->GetActorLocation() + Platform->MoveDirection * Platform->MovementSpeed * DeltaTime;
        Platform->SetActorLocation(NewLocation);
    }
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FRotateTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (Platform)
    {
        FRotator NewRotation = Platform->GetActorRotation() + Platform->RotationSpeed * DeltaTime;
        Platform->SetActorRotation(NewRotation);
    }
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FWaitWithTimerTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (Platform)
    {
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindLambda([&]
        {
            // This is a placeholder. We'll need a way to signal the StateTree to transition.
            UE_LOG(LogTemp, Warning, TEXT("Timer Finished!"));
        });
        Platform->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Platform->WaitTime, false);
    }
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FToggleVisibilityTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    APlatformBase* Platform = Cast<APlatformBase>(Context.GetOwner());
    if (Platform)
    {
        Platform->SetActorHiddenInGame(!Platform->IsHidden());
        Platform->SetActorEnableCollision(!Platform->GetActorEnableCollision());
    }
    return EStateTreeRunStatus::Succeeded;
}