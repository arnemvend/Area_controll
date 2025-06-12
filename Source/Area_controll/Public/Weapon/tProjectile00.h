// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile00.generated.h"


class UNiagaraComponent;
class USphereComponent;
class ABoom;




UCLASS()
class AREA_CONTROLL_API AtProjectile00 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile00();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) USphereComponent* Sphere;

	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) UNiagaraComponent* Niagara;

	UPROPERTY() float Damage;
	

	UPROPERTY() int Step;


	UFUNCTION() virtual void SetDamage();

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OnNiagaraSystemFinished(UNiagaraComponent* NiagaraComponent);
};
