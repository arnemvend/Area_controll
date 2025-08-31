// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "tGun13.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;




UCLASS()
class AREA_CONTROLL_API AtGun13 : public AGun
{
	GENERATED_BODY()

	AtGun13();

	virtual void TimerElapsed() override;

	virtual void Start() override;

protected:

	UPROPERTY() float Damage;
	UPROPERTY() float DownSpeed;
	UPROPERTY() float DownTime;

	UPROPERTY() int ShootEnergyPrice;


	UPROPERTY() UNiagaraComponent* BoomNiagara;
	UPROPERTY() UNiagaraSystem* BoomNSystem;

	virtual void Rotate(float Amount) override;

	virtual void BeginPlay() override;

	virtual void FreeRotate(float Amount) override;
	virtual void Fire() override;
	virtual void Tracking() override;
	virtual void StartRotateTimeline() override;

	UFUNCTION()
	void On1OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
