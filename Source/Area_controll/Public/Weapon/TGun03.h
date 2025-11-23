// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun03.generated.h"



class UNiagaraSystem;
class UNiagaraComponent;



UCLASS()
class AREA_CONTROLL_API ATGun03 : public AGun
{
	GENERATED_BODY()

public:

	ATGun03();

	virtual void TimerElapsed() override;

	virtual void Start() override;

protected:

	UPROPERTY() float Damage;
	UPROPERTY() int ShootEnergyPrice;
	UPROPERTY() bool IsFirstCheck;

	UPROPERTY() UNiagaraComponent* BoomNiagara;
	UPROPERTY() UNiagaraSystem* BoomNSystem;

	virtual void Rotate(float Amount) override;

	virtual void BeginPlay() override;

	virtual void FreeRotate(float Amount) override;
	virtual void Fire() override;
	virtual void Tracking() override;
	virtual void StartRotateTimeline() override;
	virtual void OnOverlapLogic(UPrimitiveComponent* OtherComp) override;

	/*UFUNCTION()
	void On1OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
};
