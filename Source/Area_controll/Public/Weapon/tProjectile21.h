// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile21.generated.h"


class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class ABoom;


UCLASS()
class AREA_CONTROLL_API AtProjectile21 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile21();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* NiagaraStart;
	UPROPERTY() UNiagaraComponent* NiagaraDamage;

	UPROPERTY() UNiagaraSystem* NSystem;

	UPROPERTY() USphereComponent* Sphere;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FTimerHandle Timer;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;

	UPROPERTY() float Damage;
	UPROPERTY() float TimeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FreeFall;
	UPROPERTY() float LenghtExpl;
	UPROPERTY() int Step;
	UPROPERTY() FVector CurrentSpeed;
	UPROPERTY() bool CanStart;
	


	UFUNCTION() float HorizontalDistance(FVector A, FVector B);
	UFUNCTION() void MoveToDown();
	UFUNCTION() void SparkFunc(FVector Loc);

	UFUNCTION() void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
