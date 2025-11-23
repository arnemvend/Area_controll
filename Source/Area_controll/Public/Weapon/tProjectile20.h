// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile20.generated.h"


class USphereComponent;
class UNiagaraComponent;
class ABoom;


UCLASS()
class AREA_CONTROLL_API AtProjectile20 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile20();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* NiagaraStart;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FTimerHandle Timer;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;

	UPROPERTY() float ExplRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float DelayTime;
	UPROPERTY() float Damage;

	UPROPERTY() bool IsFall;


	UFUNCTION() void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OnNiagaraFinished(UNiagaraComponent* FinishedComponent);
	UFUNCTION() virtual float HorizontalDistance(FVector A, FVector B);

};





