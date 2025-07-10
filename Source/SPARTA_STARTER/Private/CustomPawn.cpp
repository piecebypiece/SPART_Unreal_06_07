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

	GroundMoveSpeed = 3.f;
	AirMoveSpeedMultiplier = 0.5f;
	GravityAcceleration = -980.f;
	CurrentZVelocity = 0.f;
	bIsGrounded = false;

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
	ResetRotatorToGround();
}

void ACustomPawn::UpdateMovement(float Delta)
{
	if (CurrentMoveDirection.IsNearlyZero() == false)
	{
		float CurrentMoveSpeed = GroundMoveSpeed;
		if (!bIsGrounded)
		{
			CurrentMoveSpeed *= AirMoveSpeedMultiplier;
		}
		
		AddActorLocalOffset(CurrentMoveDirection * CurrentMoveSpeed * Delta);
	}
}

void ACustomPawn::CheckGroundCollision()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0, 0, -CapsuleComponent->GetScaledCapsuleHalfHeight() - 10.f); // 폰 아래로 10cm 더 트레이스

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);

	if (bHit && !bIsGrounded) // 새로 착지한 경우
	{
		OnGrounded();
	}
	bIsGrounded = bHit;
}

void ACustomPawn::OnGrounded()
{
	// 바닥 기준으로 바로 서도록 회전값 수정
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FRotator(0, CurrentRotation.Yaw, 0)); // Pitch와 Roll을 0으로 설정

	ResetRotatorToGround();
}


void ACustomPawn::UpdateLook(float Delta)
{
	// 인풋에 의해 InRotation 값이 존재한다면.
	if (RotatorRequest.IsNearlyZero() == false)
	{	// 입력값에 의한 회전 입력 적용.

		const FRotator DelRot = RotatorRequest * Delta * RotationSpeed;
		AddActorLocalRotation(FRotator(0., DelRot.Yaw, 0.)); 
		SpringArmComponent->AddRelativeRotation(DelRot);
	}
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 중력 적용
	if (bUseGravity and !bIsGrounded)
		CurrentZVelocity += GravityAcceleration * DeltaTime;

	// 지면 충돌 감지
	CheckGroundCollision();

	// 착지 시 Z축 속도 리셋
	if (bIsGrounded && CurrentZVelocity < 0.f)
	{
		CurrentZVelocity = 0.f;
	}

	// 중력 이동 적용
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(CurrentLocation + FVector(0, 0, CurrentZVelocity * DeltaTime));

	UpdateLook(DeltaTime);
	UpdateMovement(DeltaTime);
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

void ACustomPawn::ResetRotatorToGround()
{
	FRotator ActorRot = GetActorRotation(); 
	//FRotator CameraOffset = FRotator(0, 180, 0); 

	FRotator ArmRot = SpringArmComponent->GetRelativeRotation();
	SpringArmComponent->SetRelativeRotation(FRotator(ArmRot.Pitch, ActorRot.Yaw, 0.));
	//SpringArmComponent->SetWorldRotation(FRotator(0., ActorRot.Yaw, 0.));
}
