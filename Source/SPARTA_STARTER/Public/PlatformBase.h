#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformData.h"
#include "ExtendedStateTreeComponent.h"
#include "PlatformBase.generated.h"

class USplineComponent;

UCLASS()
class SPARTA_STARTER_API APlatformBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatformBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USplineComponent> PathSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UExtendedStateTreeComponent> ExtendedStateTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	TArray<FSplinePointDetails> PointDetails;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Platform State")
	int32 CurrentTargetPointIndex = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Platform State")
	bool bIsReversing = false;

private:
	float DistanceAlongSpline = 0.0f;
};