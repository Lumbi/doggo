// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "DogMovementComponent.generated.h"

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class DOGGO_API UDogMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	void InitializeComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable)
	void JumpTo(const FVector& TargetLocation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogMovement)
	float MaxSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogMovement)
	float SpeedAdjustRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogMovement)
	float JumpDuration = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogMovement)
	float JumpMargin = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogMovement)
	float JumpHopDownVerticalVelocity = 500.f;

private:
	bool IsGrounded() const;
};
