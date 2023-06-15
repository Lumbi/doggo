// Fill out your copyright notice in the Description page of Project Settings.

#include "DogAIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StaminaComponent.h"
#include "EmotionComponent.h"

void ADogAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void ADogAIController::Tick(float DeltaTime)
{
	// Update Blackboard
	auto BlackboardComponent = GetBlackboardComponent();

	APawn* ControlledPawn = GetPawn();

	if (BlackboardComponent)
	{
		// Write Player
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			BlackboardComponent->SetValueAsObject(FName(TEXT("Player")), PlayerPawn);
			BlackboardComponent->SetValueAsFloat(FName(TEXT("DistanceToPlayer")), FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation()));
			SetFocus(PlayerPawn);
		}
		else 
		{
			BlackboardComponent->ClearValue(FName(TEXT("Player")));
			BlackboardComponent->ClearValue(FName(TEXT("DistanceToPlayer")));
			ClearFocus(EAIFocusPriority::Gameplay);
		}

		// Write Stamina
		UStaminaComponent* StaminaComponent = ControlledPawn 
			? ControlledPawn->GetComponentByClass<UStaminaComponent>() : nullptr;
		if (StaminaComponent) 
		{
			BlackboardComponent->SetValueAsFloat(FName(TEXT("Stamina")), StaminaComponent->GetStamina());
			BlackboardComponent->SetValueAsFloat(FName(TEXT("StaminaRatio")), StaminaComponent->GetStaminaRatio());
		}
		else 
		{
			BlackboardComponent->ClearValue(FName(TEXT("Stamina")));
			BlackboardComponent->ClearValue(FName(TEXT("StaminaRatio")));
		}

		// Write Emotions
		UEmotionComponent* EmotionComponent = ControlledPawn
			? ControlledPawn->GetComponentByClass<UEmotionComponent>() : nullptr;
		if (EmotionComponent)
		{
			BlackboardComponent->SetValueAsFloat(FName(TEXT("Curiosity")), EmotionComponent->Curiosity);
			BlackboardComponent->SetValueAsFloat(FName(TEXT("Confidence")), EmotionComponent->Confidence);
		}
		else
		{
			BlackboardComponent->ClearValue(FName(TEXT("Curiosity")));
			BlackboardComponent->ClearValue(FName(TEXT("Confidence")));
		}
	}
}
