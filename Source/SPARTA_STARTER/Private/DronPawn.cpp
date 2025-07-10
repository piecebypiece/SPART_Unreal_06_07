


#include "DronPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h" // InputComponent 추가

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

	if (bIsFlying)
	{
		// 비행 중에는 중력의 영향을 받지 않음
		CurrentZVelocity = 0.f;
	}
}


void ADronPawn::Look(const FRotator& lookRotater)
{
	Super::Look(lookRotater);
}

void ADronPawn::Move(const FVector& MoveVector)
{
	FVector tempVector = MoveVector;
	if (bIsGrounded and MoveVector.Z < 0.)
	{
		tempVector.Z  = 0.;
	}
	else if (tempVector.Z > 0.f)
	{
		ToggleFlight(true);
	}

	Super::Move(tempVector);
}

void ADronPawn::UpdateLook(float Delta)
{
	if (bIsGrounded)
	{
		Super::UpdateLook(Delta);
		return;
	}

	// 인풋에 의해 InRotation 값이 존재한다면.
	if (RotatorRequest.IsNearlyZero() == false)
	{	// 입력값에 의한 회전 입력 적용.
		const FRotator DelRot = RotatorRequest * Delta * RotationSpeed;
		AddActorLocalRotation(DelRot); 
		SpringArmComponent->SetRelativeRotation(GetActorRotation());
	}
}

void ADronPawn::ToggleFlight(bool bInIsFly)
{
	bIsFlying = bInIsFly;

	if (bIsFlying)
	{
		CurrentZVelocity = 0.f; // 비행 시작 시 Z축 속도 초기화
		bUseGravity = false; // 비행 중에는 중력 비활성화

		// 액터 바로 뒤로 카메라를 이동시켜줍니다.
		FRotator ActorRot = GetActorRotation(); 
		SpringArmComponent->SetRelativeRotation(ActorRot);
	}
	else
	{
		bUseGravity = true; // 착지 시 중력 활성화
	}
}

void ADronPawn::OnGrounded()
{
	Super::OnGrounded();

	ToggleFlight(false);
}


