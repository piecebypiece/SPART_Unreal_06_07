// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACustomPawn::ACustomPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule Component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	//RootComponent = CapsuleComponent;

	CapsuleComponent->SetSimulatePhysics(false);

	// Skeletal Mesh Component
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent->SetSimulatePhysics(false);

	// Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->bUsePawnControlRotation = false; 
	SpringArmComponent->bInheritRoll = false;

	// Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();

	// 시작 회전 적용
	FRotator ActorRot = GetActorRotation(); 
	FRotator CameraOffset = FRotator(0, 180, 0); 

	SpringArmComponent->SetRelativeRotation(CameraOffset);
	SpringArmComponent->SetWorldRotation(ActorRot + CameraOffset);
}

FQuat ACustomPawn::CaculateRelativePichYaw(FRotator InRot)
{
	FRotator CurrentArmRelativeRotate = SpringArmComponent->GetRelativeRotation();
	FQuat CurrentQuat = CurrentArmRelativeRotate.Quaternion();

	FVector PitchAxis = CurrentQuat.GetRightVector();   // Pitch는 현재 '오른쪽' 기준 회전
	FVector YawAxis   = CurrentQuat.GetUpVector();      // Yaw는 현재 '위쪽' 기준 회전

	float PitchDelta = InRot.Pitch;
	float YawDelta   = InRot.Yaw;

	// 나머지 회전값은 현재 Roll 에 의한 회전 방영 계산
	FQuat PitchQuat = FQuat(PitchAxis, FMath::DegreesToRadians(PitchDelta));
	FQuat YawQuat   = FQuat(YawAxis,   FMath::DegreesToRadians(YawDelta));
	FQuat DeltaQuat = YawQuat * PitchQuat;
	return DeltaQuat;
}

void ACustomPawn::UpdateMovement(float Delta)
{
	if (CurrentMoveDirection.IsNearlyZero() == false)
	{
		//const FRotator actRot = GetActorRotation();
		// 방향 벡터 생성
		//const FRotationMatrix MatrixRotation(actRot);
		//const FVector ForwardDirection = MatrixRotation.GetUnitAxis(EAxis::X);
		//const FVector RightDirection = MatrixRotation.GetUnitAxis(EAxis::Y);
		//const FVector UpDirection = MatrixRotation.GetUnitAxis(EAxis::Z);

		/*FVector result = (ForwardDirection * CurrentMoveDirection.X) + (RightDirection * CurrentMoveDirection.Y)
			+ (UpDirection * CurrentMoveDirection.Z);*/

		AddActorLocalOffset(CurrentMoveDirection * MoveSpeed * Delta, true);
	}
}

void ACustomPawn::UpdateLook(float Delta)
{
	// 인풋에 의해 InRotation 값이 존재한다면.
	if (RotatorRequest.IsNearlyZero() == false)
	{	// 입력값에 의한 회전 입력 적용.
		const FRotator DelRot = RotatorRequest * Delta * RotationSpeed;
		FQuat DeltaQuat = CaculateRelativePichYaw(DelRot);

		FRotator NewRotator = DeltaQuat.Rotator();
		NewRotator.Roll = DelRot.Roll; // Roll만 수치 더해서 직접 반영

		SpringArmComponent->AddRelativeRotation(NewRotator);
		AddActorLocalRotation(FRotator(0, NewRotator.Yaw, 0)); 
	}
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLook(DeltaTime);
	UpdateMovement(DeltaTime);
}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

}

void ACustomPawn::Move(const FVector& MovementVector)
{
	FVector result = FVector::ZeroVector;
	// Mover 클래스를 정의해서 처리
	CurrentMoveDirection = MovementVector;
}

void ACustomPawn::Look(const FRotator& lookRotater)
{
	RotatorRequest = lookRotater;
}