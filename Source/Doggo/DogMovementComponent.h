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

	UFUNCTION(BlueprintPure)
	bool IsGrounded() const;

	UFUNCTION(BlueprintCallable)
	void JumpTo(const FVector& TargetLocation);

public:
	UPROPERTY(EditDefaultsOnly, Category = Walk)
	float MaxSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = Walk)
	float SpeedAdjustRate = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = Jump)
	float JumpDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = Jump)
	float JumpMargin = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = Jump)
	float JumpHopDownVerticalVelocity = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = Stamina)
	float WalkStaminaConsumptionRate = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = Stamina)
	float IdleStaminaRecoveryRate = 500.f;

private:
	class UStaminaComponent* GetStaminaComponent();
};
