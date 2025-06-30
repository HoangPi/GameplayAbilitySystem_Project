// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include "Projectile.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnImpact, bool, IsTerrain);

UCLASS(Blueprintable)
class AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	UBoxComponent *CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Direction")
	UArrowComponent* ForwardArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBehavior")
	AActor *ProjectileTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileBehavior")
	double Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileBehavior")
	double MaxTurningSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileBehavior")
	bool WaitForTarget = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	FGameplayTagContainer ProjectileTags;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayEventData payload;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Events")
	FOnImpact OnImpact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	inline void UpdateProjectileRotation(float DeltaTime);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor *OtherActor);

};
