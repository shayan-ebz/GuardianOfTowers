// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GuardianOfTowersCharacter.h"
#include "GameFramework/Actor.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetSphereRadius(400);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);


	bUseVelocityChange = false;
	MovementForce = 1000.f;
	RequiredDistanceToTarget = 100.f;
}


// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	NextPathPoint = GetNextPathPoint();
}


// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bBotSawPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("I Am Triggered"))
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
			DrawDebugString(GetWorld(), GetActorLocation(), "Target Found");
		}
		else
		{
			//keep moving toward targets
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			ForceDirection *= MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}
	}
}



FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	//UE_LOG(LogTemp, Warning, TEXT("Player Name is %s"), *PlayerPawn->GetName())

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath->PathPoints.Num() > 1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Name is %s"), *(NavPath->PathPoints[1]).ToString())
		return NavPath->PathPoints[1];
	}

	return GetActorLocation();
}

void ASTrackerBot::DamageSelf()
{

	UGameplayStatics::ApplyDamage(this, 50, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Donkey"))
			return;
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, 20, GetActorLocation(), 400, nullptr, IgnoredActors, this, GetInstigatorController()
		, true);

	Destroy();
}

void ASTrackerBot::HandleTakenDamage()
{
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
}


void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bSelfDestruct)
	{
		auto PlayerPawn = Cast<AGuardianOfTowersCharacter>(OtherActor);

		if (PlayerPawn)
		{
			GetWorldTimerManager().SetTimer(Timer_SelfDamage, this, &ASTrackerBot::SelfDestruct, 1.f, true, 0.f);
		}
		bSelfDestruct = true;
	}
}




