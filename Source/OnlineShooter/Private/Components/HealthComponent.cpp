// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY(LogHealthComponent)

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	CurrentHealth = MaxHealth;
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const auto Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogHealthComponent, Error, TEXT("No owner specified!"));
		return;
	}

	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	SetHealth(MaxHealth);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	const auto Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogHealthComponent, Error, TEXT("No owner specified!"));
	}
	else
	{
		HealthChangedEvent.Clear();
		Owner->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::TakeDamage);
		DeathEvent.Clear();
	}
	Super::EndPlay(EndPlayReason);
}

void UHealthComponent::LogData()
{
	if (GetOwner())
	{
		UE_LOG(LogHealthComponent, Warning, TEXT("%s.HealthComponent data begin ---------|"), *GetOwner()->GetHumanReadableName());
		UE_LOG(LogHealthComponent, Display, TEXT("Max Health = %f"), MaxHealth);
		UE_LOG(LogHealthComponent, Display, TEXT("Current Health = %f"), CurrentHealth);
		UE_LOG(LogHealthComponent, Warning, TEXT("%s.HealthComponent data end ---------|"), *GetOwner()->GetHumanReadableName());
	}
	
}

void UHealthComponent::LogState()
{
	if (GetOwner())
	{
		UE_LOG(LogHealthComponent, Warning, TEXT("%s.HealthComponent state begin ---------|"), *GetOwner()->GetHumanReadableName());
		UE_LOG(LogHealthComponent, Display, TEXT("%s damage applied to owner"),
			GetOwner()->OnTakeAnyDamage.Contains(this, "TakeDamage") ? *FString("Process") : *FString("Don't process"));
		UE_LOG(LogHealthComponent, Display, TEXT("Health Event %s"),
			HealthChangedEvent.IsBound() ? TEXT("is bound") : TEXT("is not bound"));
		UE_LOG(LogHealthComponent, Display, TEXT("Death Event %s"),
			DeathEvent.IsBound() ? TEXT("is bound") : TEXT("is not bound"));
		UE_LOG(LogHealthComponent, Warning, TEXT("%s.HealthComponent state end ---------|"), *GetOwner()->GetHumanReadableName());
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const auto ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	CurrentHealth = ClampedHealth;
	HealthChangedEvent.Broadcast(CurrentHealth);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetOwner() && DamagedActor==GetOwner())
	{
		SetHealth(CurrentHealth-Damage);

		LogData();
		LogState();
		if (CurrentHealth == 0.f)
		{
			DeathEvent.Broadcast(InstigatedBy, DamageCauser);
		}
	}
}

