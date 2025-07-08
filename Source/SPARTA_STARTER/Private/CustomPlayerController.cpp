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
	// 인풋에 의해 InRotation 값이 존재한다면.
	if (InRotation.IsNearlyZero() == false)
	{	// 입력값에 의한 회전 입력 적용.
		FRotator currentRotation = GetControlRotation();
		currentRotation += InRotation * RotationSpeed * delta;
		SetControlRotation (currentRotation);

		const FRotator YawRotation(0, currentRotation.Yaw, 0);	// 세로축 기준 회전 생성 위아래 회전은 액터회전에 반영하지 않는다.
		ControlledPawn->Look(YawRotation);
	}
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

	// Pawn의 현재 회전을 가져옴
	FRotator PawnRotation = InPawn->GetActorRotation();

	// Yaw만 사용 (Pitch, Roll은 보통 카메라용)
	FRotator NewControlRotation = FRotator(0.f, PawnRotation.Yaw, 0.f);

	SetControlRotation(NewControlRotation);

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
		FVector result = FVector::ZeroVector;
		if (MovementVector.IsNearlyZero() == false)
		{

			const FRotator Rotation = GetControlRotation();	// 컨트롤러의 회전 값.
			const FRotator YawRotation(0, Rotation.Yaw, 0);	// 세로축 기준 회전 생성
			
			// 방향 벡터 생성
			const FRotationMatrix MatrixYRotation(YawRotation);
			const FVector ForwardDirection = MatrixYRotation.GetUnitAxis(EAxis::X);
			const FVector RightDirection = MatrixYRotation.GetUnitAxis(EAxis::Y);
			const FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

			result = (ForwardDirection * MovementVector.X) + (RightDirection * MovementVector.Y) 
				+ (UpDirection * MovementVector.Z) ;
		}
		ControlledPawn->Move(result);
	}
}

void ACustomPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	InRotation.Yaw = LookAxisVector.X;
	InRotation.Pitch = LookAxisVector.Y;
}

void ACustomPlayerController::HandleUpDown(const FInputActionValue& Value)
{
	TObjectPtr<ADronPawn> DronPawn = Cast<ADronPawn>(GetPawn());

	if (DronPawn.IsNull())
		return;
	
	const float MovementValue = Value.Get<float>();
	FVector result = FVector::ZeroVector;
	if (FMath::IsNearlyEqual(MovementValue, 0.0f) == false)
	{
		const FRotator Rotation = GetControlRotation();	// 컨트롤러의 회전 값.
		// 새로축을 기준으로 한 방향 생성
		const FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);
		result = UpDirection * MovementValue;
	}
	ControlledPawn->Move(result);
}
