#include "SHealthComponent.h"
#include "STrackerBot.h"
#include "GameFramework/Actor.h"


USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.f;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = DefaultHealth;

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}

}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	//update health
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health is %f"), Health)

		if (Health <= 0)
		{
			auto TrackerBot = Cast<ASTrackerBot>(DamagedActor);
			if (TrackerBot)
			{
				TrackerBot->SelfDestruct();
			}
		}

	auto TrackerBot = Cast<ASTrackerBot>(DamagedActor);
	if (TrackerBot)
	{
		TrackerBot->HandleTakenDamage();
	}
}
