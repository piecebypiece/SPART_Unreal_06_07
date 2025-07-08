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
	void Move(const FVector& MovementVector);

	UFUNCTION(BlueprintCallable)
	void Look(const FRotator& LookRotater);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MoveProperties")
	float MoveSpeed = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MoveProperties")
	FVector CurrentMoveDirection = FVector::ZeroVector;
};



/*



## 4️⃣ 도전 과제 (선택 요구 사항) 🦅

### **도전 과제 1번 - 6자유도 (6 DOF) 드론/비행체 구현 (난이도 상)**

- **6축 이동 및 회전 액션 구현**
- **이동**
- 전/후 (W/S) - 로컬 X축 이동
- 좌/우 (A/D) - 로컬 Y축 이동
- 상/하 (Space/Shift) - 로컬 Z축 이동
- **회전**
- Yaw - 좌우 회전, 마우스 X축 이동
- Pitch - 상하 회전, 마우스 Y축 이동
- Roll - 기울기 회전, 마우스 휠 또는 별도 키
- **Orientation 기반 이동 구현**
- 현재 Pawn의 회전 상태에 따라 이동 방향이 결정되는 비행체 움직임을 구현합니다.
- 단순 월드 좌표계 이동이 아닌, Pawn의 로컬 좌표계 기준 이동을 구현합니다.

### **도전 과제 2번 - 중력 및 낙하 구현 (난이도 최상)**

- **인공 중력 구현**
- **Tick 함수**를 통해 매 프레임 중력 가속도를 직접 계산합니다.
- 적절한 중력 상수 (예: -980 cm/s²)를 사용하여 낙하 속도를 구현합니다.
- LineTrace 또는 SweepTrace를 사용하여 지면 충돌을 감지합니다.
- 착지 시 Z축 속도를 0으로 초기화합니다.
- **에어컨트롤 구현 (공중 WASD 제어)**
- 공중에서는 지상 이동속도의 30~50% 정도로 제한합니다.
- 지상/공중 상태에 따라 이동 로직을 구분하여 자연스러운 움직임을 구현합니다.
*/

/*
1. Pawn 클래스 생성
2. 충돌 컴포넌트를 루트 컴포넌트로 설정합니다 (CapsuleComponent/BoxComponent/SphereComponent 중 택 1).
3. SkeletalMeshComponent, SpringArmComponent, CameraComponent를 부착하여 3인칭 시점을 구성합니다.
4. GameMode에서 DefaultPawnClass를 이 Pawn 클래스로 지정합니다.
*/