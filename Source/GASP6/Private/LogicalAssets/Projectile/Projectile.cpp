// Fill out your copyright notice in the Description page of Project Settings.

#include "LogicalAssets/Projectile/Projectile.h"

#include "Kismet/KismetMathLibrary.h"
#include "Ultilities/MyMacros.h"
#include "Tags/MyTagManager.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetLifeSpan(3.0f);

	// Set up the collision box
	this->CollisionBox = this->CreateDefaultSubobject<UBoxComponent>("My_Collision_Box");
	this->RootComponent = this->CollisionBox;
	this->CollisionBox->SetBoxExtent(FVector(20.0f));
	this->CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel2);

	// Set up forward arrow
	this->ForwardArrow = this->CreateDefaultSubobject<UArrowComponent>("My_Foward_Arrow");
	this->ForwardArrow->SetupAttachment(this->RootComponent);
	this->ForwardArrow->SetWorldRotation(this->GetActorRotation());

	// Set up attributes
	this->Speed = 850.0f;
	this->MaxTurningSpeed = 48.0f;
	this->Damage = 20.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	this->payload.EventMagnitude = this->Damage;
	this->payload.TargetTags = this->ProjectileTags;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(this->WaitForTarget && !this->ProjectileTarget)
	{
		return;
	}
	if (this->ProjectileTarget)
	{
		this->UpdateProjectileRotation(DeltaTime);
	}
	this->SetActorLocation(
		this->GetActorLocation() + this->ForwardArrow->GetForwardVector() * DeltaTime * this->Speed,
		true
	);
}

void AProjectile::UpdateProjectileRotation(float DeltaTime)
{
	FVector TargetLocation = this->ProjectileTarget->GetActorLocation();
	FVector ActorLocation = this->GetActorLocation();

	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation);
	FRotator CurrentRotation = this->GetActorRotation();
	FRotator Diff = UKismetMathLibrary::NormalizedDeltaRotator(DesiredRotation, CurrentRotation);
	double min = -1 * this->MaxTurningSpeed * DeltaTime;
	double max = this->MaxTurningSpeed * DeltaTime;
	this->SetActorRotation(
		FRotator(
			CurrentRotation.Pitch + CLAMP(Diff.Pitch, max, min),
			CurrentRotation.Yaw + CLAMP(Diff.Yaw, max, min),
			CurrentRotation.Roll + CLAMP(Diff.Roll, max, min)
		)
	);
}

void AProjectile::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if(OtherActor)
	{
		if(UAbilitySystemComponent *asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
		{
			asc->HandleGameplayEvent(MyTags::Ability::attacked, &this->payload);
		}
	}
	this->Destroy();
}