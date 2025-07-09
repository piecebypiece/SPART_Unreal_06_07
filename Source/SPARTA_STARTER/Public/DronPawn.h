

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

protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Look(const FRotator& lookRotater) override;

	virtual void Move(const FVector& MoveVector) override;

	virtual void UpdateLook (float Delta) override;
private:


public:	

};
