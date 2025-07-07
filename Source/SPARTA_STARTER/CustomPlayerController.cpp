// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CustomPawn.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ControlledPawn = Cast<ACustomPawn>(GetPawn());
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandleMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandleMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandleLook);
	}
}

void ACustomPlayerController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		// Pawn의 현재 회전을 가져옴
		FRotator PawnRotation = InPawn->GetActorRotation();

		// Yaw만 사용 (Pitch, Roll은 보통 카메라용)
		FRotator NewControlRotation = FRotator(0.f, PawnRotation.Yaw, 0.f);

		SetControlRotation(NewControlRotation);
	}
}

void ACustomPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (ControlledPawn)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		FVector result = FVector::ZeroVector;
		if (MovementVector.IsNearlyZero() == false)
		{

			const FRotator Rotation = GetControlRotation();	// 컨트롤러의 회전 값.
			const FRotator YawRotation(0, Rotation.Yaw, 0);	// 세로축 기준 회전 생성
			// 새로축을 기준으로 한 방향 생성
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			result = (ForwardDirection * MovementVector.X) + (RightDirection * MovementVector.Y);
		}
		ControlledPawn->Move(result);
	}
}

void ACustomPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);

	const FRotator Rotation = GetControlRotation();	// 컨트롤러의 회전 값.
	const FRotator YawRotation(0, Rotation.Yaw, 0);	// 세로축 기준 회전 생성
	ControlledPawn->Look(YawRotation);
}
