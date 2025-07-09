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
	float MoveSpeed = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InputProperties")
	FVector CurrentMoveDirection = FVector::ZeroVector;

	FQuat CaculateRelativePichYaw(FRotator InRot);

	virtual void UpdateMovement(float Delta);
	virtual void UpdateLook(float Delta);

public:	
	virtual void Tick(float DeltaTime) override;
	// 입력에 따른 키 바인딩은 PlayerController에 있는것이 자연스러울것 같아 이동시켰습니다.
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

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

};