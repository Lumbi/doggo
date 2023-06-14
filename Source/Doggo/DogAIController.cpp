// Fill out your copyright notice in the Description page of Project Settings.

#include "DogAIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StaminaComponent.h"

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

	if (BlackboardComponent)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		// Write Player
		if (PlayerPawn)
		{
			BlackboardComponent->SetValueAsObject(FName(TEXT("Player")), PlayerPawn);
			SetFocus(PlayerPawn);
		}
		else {
			BlackboardComponent->ClearValue(FName(TEXT("Player")));
			ClearFocus(EAIFocusPriority::Gameplay);
		}

		// Write Stamina
		APawn* ControlledPawn = GetPawn();
		UStaminaComponent* StaminaComponent = ControlledPawn 
			? ControlledPawn->GetComponentByClass<UStaminaComponent>() : nullptr;
		if (StaminaComponent) {
			BlackboardComponent->SetValueAsFloat(FName(TEXT("Stamina")), StaminaComponent->GetStamina());
			BlackboardComponent->SetValueAsFloat(FName(TEXT("StaminaRatio")), StaminaComponent->GetStaminaRatio());
		}
		else {
			BlackboardComponent->ClearValue(FName(TEXT("Stamina")));
			BlackboardComponent->ClearValue(FName(TEXT("StaminaRatio")));
		}
	}
}
