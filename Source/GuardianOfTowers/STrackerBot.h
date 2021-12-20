// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class GUARDIANOFTOWERS_API ASTrackerBot : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

	void HandleTakenDamage();

	void SelfDestruct();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	void DamageSelf();


protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* SphereComp;

	class UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TrackerBot")
	bool bBotSawPlayer;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TrackerBot")
	FVector NextPathPoint;

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	FTimerHandle Timer_SelfDamage;

	bool bSelfDestruct;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

};
