// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "Projectile_2.generated.h"



class UNiagaraComponent;
class UNiagaraSystem;
class UBoxComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class ABoom;



UCLASS()
class AREA_CONTROLL_API AProjectile_2 : public AProjectile
{
	GENERATED_BODY()

public:

	AProjectile_2();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* MeshShadow;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY() UNiagaraSystem* NSystem;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FRotator CurrentRot;
	UPROPERTY() FRotator AimRot;
	UPROPERTY() float CurrentHigh;
	UPROPERTY() float Damage;
	UPROPERTY() float HighStep;
	UPROPERTY() float Speed;
	UPROPERTY() float MaxHealth;
	UPROPERTY() float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TimeOfFall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float RotateMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CustomHigh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CustomDeltaTime;
	UPROPERTY() int CurrentStep;
	UPROPERTY() int FinishStep;
	UPROPERTY() bool CanLight;

	UPROPERTY() FTimerHandle FallTimer;
	UPROPERTY() FTimerHandle WayTimer;
	UPROPERTY() FTimerHandle CanLightTimer;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage0, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
};
