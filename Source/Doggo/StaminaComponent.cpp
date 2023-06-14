// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	Stamina = MaxStamina;
}
\
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// DEBUG
	{
		DrawDebugCylinder(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 150.f * GetStaminaRatio()),
			15.f,
			12,
			FColor::Green
		);
	}
}

void UStaminaComponent::ConsumeStamina(float Rate)
{
	Stamina = FMath::Clamp(Stamina - Rate * GetWorld()->GetDeltaSeconds(), 0.f, MaxStamina);
}

void UStaminaComponent::RecoverStamina(float Rate)
{
	Stamina = FMath::Clamp(Stamina + Rate * GetWorld()->GetDeltaSeconds(), 0.f, MaxStamina);
}
