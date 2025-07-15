#include "PlatformBase.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StateTreeComponent.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->SetupAttachment(RootComponent);

	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
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

	// Set the initial position of the platform to the start of the spline
	if (PathSpline->GetNumberOfSplinePoints() > 0)
	{
		SetActorLocation(PathSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	}
}

void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathSpline->GetNumberOfSplinePoints() < 2 || CurrentTargetPointIndex < 0 || CurrentTargetPointIndex >= PointDetails.Num())
	{
		return; 
	}
	// 경로 속도
	const float CurrentSpeed = PointDetails[CurrentTargetPointIndex].Speed;
	DistanceAlongSpline += CurrentSpeed * DeltaTime;

	// 스플라인의 새 경로 찾기
	const FVector NewLocation = PathSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	const FRotator NewRotation = PathSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	SetActorLocation(NewLocation);

	// 경로별 회전
	const FRotator TargetRotation = PointDetails[CurrentTargetPointIndex].TargetRotation;
	const FRotator SmoothedRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 2.0f); // 2.0f is interp speed
	SetActorRotation(SmoothedRotation);


	const float TargetDistance = PathSpline->GetDistanceAlongSplineAtSplinePoint(CurrentTargetPointIndex);
	if (DistanceAlongSpline >= TargetDistance)
	{
		// TODO : StateTree에 경로에 도달했음을 알림.
		UE_LOG(LogTemp, Warning, TEXT("Reached point %d"), CurrentTargetPointIndex);
	}
}
