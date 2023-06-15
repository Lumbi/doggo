// Fill out your copyright notice in the Description page of Project Settings.


#include "DogMovementComponent.h"
#include "StaminaComponent.h"

void UDogMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	bUseAccelerationForPaths = true;
}

float UDogMovementComponent::GetMaxSpeed() const
{
	return MaxSpeed;
}

void UDogMovementComponent::JumpTo(const FVector& TargetLocation)
{
	// Compute initial velocity
	FVector StartLocation = GetActorFeetLocation();
	const float HalfHeight = GetActorLocation().Z - GetActorFeetLocation().Z;
	Velocity.X = (TargetLocation.X - StartLocation.X) / JumpDuration;
	Velocity.Y = (TargetLocation.Y - StartLocation.Y) / JumpDuration;
	if (TargetLocation.Z > StartLocation.Z) {
		const float JumpHeight = (TargetLocation.Z - StartLocation.Z) + HalfHeight + JumpMargin;
		Velocity.Z = FMath::Sqrt(FMath::Abs(2.f * GetGravityZ() * JumpHeight));
	}
	else {
		Velocity.Z = JumpHopDownVerticalVelocity;
	}

	UpdateComponentVelocity();
}

void UDogMovementComponent::StartResting()
{
	bIsResting = true;
}

void UDogMovementComponent::StopResting()
{
	bIsResting = false;
}

UStaminaComponent* UDogMovementComponent::GetStaminaComponent() const
{
	if (!PawnOwner) { return nullptr; }
	return PawnOwner->GetComponentByClass<UStaminaComponent>();
}

bool UDogMovementComponent::IsGrounded() const
{
	if (!PawnOwner) { return true; }

	const float GroundCheckSphereRadius = 10.f;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PawnOwner);
	const bool bDidHitGround = GetWorld()->SweepSingleByChannel(
		Hit,
		GetActorFeetLocation(), // start
		GetActorFeetLocation(), // end
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeSphere(GroundCheckSphereRadius),
		Params
	);

	const bool bHasUpwardVelocity = Velocity.Z > 0.f;

	// DEBUG
	{
		DrawDebugSphere(GetWorld(), GetActorFeetLocation(), GroundCheckSphereRadius, 12, FColor::Yellow);
		UE_LOG(LogTemp, Warning, TEXT("[IsGrounded] bDidHitGround: %d bHasUpwardVelocity: %d"), bDidHitGround, bHasUpwardVelocity);
		UE_LOG(LogTemp, Warning, TEXT("[IsGrounded] Velocity = %s"), *Velocity.ToString());
	}

	return bDidHitGround && !bHasUpwardVelocity;
}

bool UDogMovementComponent::IsResting() const
{
	return bIsResting;
}

void UDogMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime)) { return; }

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent) { return; }

	UStaminaComponent* StaminaComponent = GetStaminaComponent();

	// Walk / Idle State
	if (IsGrounded()) {
		// Apply Acceleration
		FVector InputVector = ConsumeInputVector();
		FVector TargetVelocity = InputVector * GetMaxSpeed();
		Velocity = FMath::Lerp(Velocity, TargetVelocity, DeltaTime * SpeedAdjustRate);

		// Prevent upward movement
		if (Velocity.Z > 0.f) { Velocity.Z = 0.f; }

		// Clamp to max speed
		Velocity = Velocity.GetClampedToMaxSize(GetMaxSpeed());

		// Update stamina
		{
			if (StaminaComponent) {
				if (!Velocity.IsNearlyZero(50.f))
				{
					StaminaComponent->ConsumeStamina(WalkStaminaConsumptionRate);
				}
				else {
					StaminaComponent->RecoverStamina(IdleStaminaRecoveryRate);
				}
			}
		}
	}
	
	// Jumping / Falling State
	if (!IsGrounded())
	{
		// Apply gravity
		Velocity.Z += GetGravityZ() * DeltaTime;
	}

	// Stop resting if moving and stamina is full
	if (IsResting()) {
		if (!Velocity.IsNearlyZero() && StaminaComponent && StaminaComponent->GetStaminaRatio() >= 1.f)
		{
			StopResting();
		}
	}

	// Apply Velocity
	{
		FVector Movement = Velocity * DeltaTime;
		if (!Movement.IsNearlyZero())
		{
			const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
			const FQuat CurrentRotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit;
			SafeMoveUpdatedComponent(Movement, CurrentRotation, true, Hit);

			// Depenetrate
			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Movement);
				SlideAlongSurface(Movement, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}
		}
	}

	// Face movement direction
	{

		FVector TargetFaceDirection = Velocity;
		TargetFaceDirection.Z = 0.f;
		if (!TargetFaceDirection.IsNearlyZero()) {
			FRotator TargetFaceRotation = TargetFaceDirection.ToOrientationRotator();
			FRotator CurrentFaceRotation = UpdatedComponent->GetComponentRotation();
			UpdatedComponent->SetWorldRotation(FMath::Lerp(CurrentFaceRotation, TargetFaceRotation, DeltaTime * RotationAdjustRate));
		}
	}

	// DEBUG
	{
		DrawDebugDirectionalArrow(
			GetWorld(),
			PawnOwner->GetActorLocation(),
			PawnOwner->GetActorLocation() + Velocity,
			10.f,
			FColor::Red,
			false,
			-1.0f,
			0,
			5.0f
		);
	}

	UpdateComponentVelocity();
}
