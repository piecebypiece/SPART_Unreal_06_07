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
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true; 

	// Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomPawn::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (CurrentMoveDirection != FVector::Zero())
	{
		AddActorWorldOffset(CurrentMoveDirection * MoveSpeed, true);
	}
}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

}

void ACustomPawn::Move(const FVector& movementVector)
{
	CurrentMoveDirection = movementVector;
}

void ACustomPawn::Look(const FRotator& lookRotater)
{
	SetActorRotation(lookRotater);
}