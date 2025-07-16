#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlatformData.generated.h"

UENUM(BlueprintType)
enum class EPlatformMovementMode : uint8
{
	None UMETA(DisplayName = "None"),
	Loop UMETA(DisplayName = "Loop"),
	PingPong UMETA(DisplayName = "PingPong"),
};

UENUM(BlueprintType)
enum class EPlatformAction : uint8
{
	None UMETA(DisplayName = "None"),
	Pause UMETA(DisplayName = "Pause"),
	Disappear UMETA(DisplayName = "Disappear"),
	Reappear UMETA(DisplayName = "Reappear"),
};

USTRUCT(BlueprintType)
struct FSplinePointDetails : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	EPlatformAction Action = EPlatformAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform", meta = (EditCondition = "Action == EPlatformAction::Pause"))
	float PauseDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	bool bDisappear;
};
