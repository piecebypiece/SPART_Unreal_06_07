

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
