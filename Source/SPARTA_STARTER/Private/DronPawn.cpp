


#include "DronPawn.h"


// Sets default values
ADronPawn::ADronPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADronPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


