


#include "DronPawn.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ADronPawn::ADronPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADronPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADronPawn::Look(const FRotator& lookRotater)
{
	Super::Look(lookRotater);
	// 모든 회전을 그대로 수신.
	//SetActorRotation(lookRotater);
}

void ADronPawn::Move(const FVector& MoveVector)
{
	Super::Move(MoveVector);
}

void ADronPawn::UpdateLook(float Delta)
{
	// 인풋에 의해 InRotation 값이 존재한다면.
	if (RotatorRequest.IsNearlyZero() == false)
	{	// 입력값에 의한 회전 입력 적용.
		const FRotator DelRot = RotatorRequest * Delta * RotationSpeed;
		FQuat DeltaQuat = CaculateRelativePichYaw(DelRot);

		FRotator NewRotator = DeltaQuat.Rotator();
		NewRotator.Roll = DelRot.Roll; // Roll만 수치 더해서 직접 반영

		AddActorLocalRotation(NewRotator); 
		SpringArmComponent->SetRelativeRotation(GetActorRotation());
	}
}


