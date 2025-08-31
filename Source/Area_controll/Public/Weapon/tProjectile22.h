// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile22.generated.h"


class USphereComponent;
class UMaterialBillboardComponent;
class UMaterialInterface;
class UNiagaraComponent;
class ABoom;


UCLASS()
class AREA_CONTROLL_API AtProjectile22 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile22();

	UFUNCTION() void Start();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* NiagaraStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialBillboardComponent* Billboard;

	UPROPERTY() UMaterialInterface* Material;

	UPROPERTY() ABoom* BoomActor;


	UPROPERTY() float Damage;
	UPROPERTY() float Accurary;

	UPROPERTY() float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DTime;

	UPROPERTY() FTimerHandle Timer;


	UFUNCTION() void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
