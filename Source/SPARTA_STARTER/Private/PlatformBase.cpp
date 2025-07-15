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

	if (PathSpline->GetNumberOfSplinePoints() > 0)
	{
		SetActorLocation(PathSpline->GetLocationAtSplinePoint(CurrentTargetPointIndex, ESplineCoordinateSpace::World));
	}
}

void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathSpline->GetNumberOfSplinePoints() < 2 || CurrentTargetPointIndex < 0 || CurrentTargetPointIndex >= PathSpline->GetNumberOfSplinePoints())
	{
		return;
	}

	const FSplinePointDetails& CurrentPointDetail = PointDetails[CurrentTargetPointIndex];

	const FVector TargetLocation = PathSpline->GetLocationAtSplinePoint(CurrentTargetPointIndex, ESplineCoordinateSpace::World);
	const FRotator TargetRotation = CurrentPointDetail.TargetRotation;

	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, CurrentPointDetail.Speed / 100.0f);
	SetActorLocation(NewLocation);

	FRotator NewActorRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
	SetActorRotation(NewActorRotation);

	if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(10.0f))
	{
	}
}
