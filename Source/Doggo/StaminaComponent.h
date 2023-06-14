// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOGGO_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	bool HasStamina() const { return Stamina > 0.f && Stamina <= MaxStamina; }

	UFUNCTION(BlueprintPure)
	float GetStamina() const { return Stamina; }

	UFUNCTION(BlueprintPure)
	float GetStaminaRatio() const { return Stamina / MaxStamina; }

	UFUNCTION(BlueprintCallable)
	void ConsumeStamina(float Rate);

	UFUNCTION(BlueprintCallable)
	void RecoverStamina(float Rate);

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxStamina = 1000.f;

	UPROPERTY(VisibleAnywhere, Transient)
	float Stamina = 0.f;
};
