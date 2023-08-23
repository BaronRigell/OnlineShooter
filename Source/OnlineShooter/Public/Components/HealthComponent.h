// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ILogable.h"
#include "HealthComponent.generated.h"

DECLARE_EVENT_OneParam(UHealthComponent, FHealthChangedEventSignature, float)
DECLARE_EVENT_TwoParams(UHealthComponent, FDeathEventSignature, AController*, AActor*)

DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Display, Display)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ONLINESHOOTER_API UHealthComponent : public UActorComponent, public IILogable
{
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.f;

	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	FDeathEventSignature& OnDeath() {return DeathEvent;}
	FHealthChangedEventSignature& OnHealthChanged() {return HealthChangedEvent;}

	virtual void LogData() override;
	virtual void LogState() override;
private:
	float CurrentHealth;
	
	void SetHealth(float NewHealth);
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );
	
	FDeathEventSignature DeathEvent;
	FHealthChangedEventSignature HealthChangedEvent;
};
