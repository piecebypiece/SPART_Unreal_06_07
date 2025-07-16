#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Task6Character.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SPARTA_STARTER_API ATask6Character : public ACharacter
{
	GENERATED_BODY()

public:
	ATask6Character();

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float JumpVelocity = 600.0f;
};
