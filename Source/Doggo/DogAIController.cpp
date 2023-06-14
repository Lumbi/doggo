// Fill out your copyright notice in the Description page of Project Settings.

#include "DogAIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent)
	{
		if (PlayerPawn)
		{
			BlackboardComponent->SetValueAsObject(FName(TEXT("Player")), PlayerPawn);
			SetFocus(PlayerPawn);
		}
		else {
			BlackboardComponent->ClearValue(FName(TEXT("Player")));
			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
