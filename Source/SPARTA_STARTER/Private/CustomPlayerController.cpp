// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CustomPawn.h"
#include "DronPawn.h"
void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ACustomPlayerController::Tick(float delta)
{
	Super::Tick(delta);
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACustomPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr)
		return;

	TObjectPtr<ACustomPawn> CustomPawn = Cast<ACustomPawn>(GetPawn());
	ControlledPawn = CustomPawn;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
		return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem == nullptr)
	{
		return;
	}
	Subsystem->ClearAllMappings();


	if (TObjectPtr<ADronPawn> DronPawn = Cast<ADronPawn>(GetPawn()))
	{
		Subsystem->AddMappingContext(DronMappingContext, 0);

		//// Up, Down 액션 처리
		//EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandleUpDown);
		//EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandleUpDown);
	}
	else
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// 둘은 같은 액션을 공유하기 때문에 변경해줄 필요는 없다
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandleMove);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandleMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::HandleLook);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &ACustomPlayerController::HandleLook);
}

void ACustomPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (ControlledPawn)
	{
		const FVector MovementVector = Value.Get<FVector>();
		ControlledPawn->Move(MovementVector);
	}
}

void ACustomPlayerController::HandleLook(const FInputActionValue& Value)
{
	if (ControlledPawn)
	{
		const FVector LookAxisVector = Value.Get<FVector>();
		FRotator Rotate(LookAxisVector.Y, LookAxisVector.X, LookAxisVector.Z);
		ControlledPawn->Look(Rotate);
	}
}