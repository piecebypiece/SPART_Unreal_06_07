

#pragma once

#include "CoreMinimal.h"
#include "CustomPawn.h"
#include "DronPawn.generated.h"

UCLASS()
class SPARTA_STARTER_API ADronPawn : public ACustomPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	bool bIsFlying = false; // 비행 중 여부

	UFUNCTION(BlueprintCallable)
	void ToggleFlight(bool bIsFly); // 비행 모드 전환

protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Look(const FRotator& lookRotater) override;

	virtual void Move(const FVector& MoveVector) override;

	virtual void UpdateLook (float Delta) override;

	virtual void OnGrounded() override; // CustomPawn의 OnGrounded 오버라이드
private:


public:	

};
