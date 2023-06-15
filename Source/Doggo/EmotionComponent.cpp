// Fill out your copyright notice in the Description page of Project Settings.


#include "EmotionComponent.h"

UEmotionComponent::UEmotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEmotionComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UEmotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEmotionComponent::AffectCuriosity(float Rate)
{
	Curiosity = FMath::Clamp(Curiosity + Rate * GetWorld()->GetDeltaSeconds(), 0.f, 1.f);
}

void UEmotionComponent::AffectConfidence(float Rate)
{
	Confidence = FMath::Clamp(Confidence + Rate * GetWorld()->GetDeltaSeconds(), 0.f, 1.f);
}

