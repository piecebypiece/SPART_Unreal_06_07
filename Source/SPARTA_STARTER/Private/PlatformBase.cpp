#include "PlatformBase.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "StateTree.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->SetupAttachment(RootComponent);

	ExtendedStateTreeComponent = CreateDefaultSubobject<UExtendedStateTreeComponent>(TEXT("ExtendedStateTreeComponent"));
}

void APlatformBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const int32 SplinePoints = PathSpline->GetNumberOfSplinePoints();
	if (PointDetails.Num() != SplinePoints)
	{
		PointDetails.SetNum(SplinePoints);
	}
}

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();

	if (ExtendedStateTreeComponent && ExtendedStateTreeComponent->GetStateTree())
	{
		ExtendedStateTreeComponent->StartLogic();
	}
}

void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bHasCollisionOccurredThisTick = false;
}

void APlatformBase::OnPlatformHit()
{
	bHasCollisionOccurredThisTick = true;
}
