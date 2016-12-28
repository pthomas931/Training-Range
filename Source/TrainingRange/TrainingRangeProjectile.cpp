// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TrainingRange.h"
#include "TrainingRangeProjectile.h"
#include "TrainingRangeCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ATrainingRangeProjectile::ATrainingRangeProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATrainingRangeProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.5f;
	ProjectileMovement->ProjectileGravityScale = 0.8f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ATrainingRangeProjectile::init(float firePower, FLinearColor projColor) {
	if (firePower < 1) {
		firePower = (firePower > 0.25f ? firePower : 0.25f);

		ProjectileMovement->Velocity = ProjectileMovement->Velocity * firePower;
	}

	projectileColor = projColor;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, projectileColor.ToString());
}

void ATrainingRangeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovement->Velocity = ProjectileMovement->Velocity / 2;

	if ((OtherActor != NULL) && (OtherActor != this))
	{
		ATrainingRangeCharacter* hitChar = Cast<ATrainingRangeCharacter>(OtherActor);

		FDamageEvent DmgEvent = FDamageEvent();

		if (hitChar != NULL) {
			hitChar->TakeDamage(1.0, DmgEvent, NULL, GetOwner());
		}
	}

	// If physics object add impulse (or a push)
	if ((OtherComp != NULL) && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulseAtLocation(GetVelocity() * 50.0f, GetActorLocation());
	}

	//Destroy();
}