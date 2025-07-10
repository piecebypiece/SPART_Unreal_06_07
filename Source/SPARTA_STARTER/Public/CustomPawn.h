// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS(BlueprintType)
class SPARTA_STARTER_API ACustomPawn : public APawn
{
	GENERATED_BODY()

public:
	ACustomPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InputProperties")
	FRotator RotatorRequest = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputProperties", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputProperties",  meta = (AllowPrivateAccess = "true"))
	float GroundMoveSpeed = 3.f; // 지상 이동 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputProperties",  meta = (AllowPrivateAccess = "true"))
	float AirMoveSpeedMultiplier = 0.5f; // 공중 이동 속도 제한 (지상 속도의 비율)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float GravityAcceleration = -980.f; // 중력 가속도 (cm/s^2)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector CurrentMoveDirection = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentZVelocity = 0.f; // 현재 Z축 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsGrounded = false; // 지상 착지 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bUseGravity = true; // 중력 적용 여부

	virtual void UpdateMovement(float Delta);
	virtual void UpdateLook(float Delta);
	virtual void CheckGroundCollision(); // 지면 충돌 감지 함수
	virtual void OnGrounded();

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UFUNCTION(BlueprintCallable)
	virtual void Move(const FVector& MovementVector);

	UFUNCTION(BlueprintCallable)
	virtual void Look(const FRotator& LookRotater);

	void ResetRotatorToGround();
};