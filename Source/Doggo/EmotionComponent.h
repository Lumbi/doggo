// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmotionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOGGO_API UEmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEmotionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void AffectCuriosity(float Rate);

	UFUNCTION(BlueprintCallable)
	void AffectConfidence(float Rate);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Curiosity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Fear = 0.f;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Confidence = 0.5f;
};
