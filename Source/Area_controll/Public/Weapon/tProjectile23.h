// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile23.generated.h"



class ABoom;
class ATower;
class USphereComponent;
class UBoxComponent;
class UNiagaraComponent;




UCLASS()
class AREA_CONTROLL_API AtProjectile23 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile23();

	UFUNCTION() void Start();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* ExtSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* IntSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* NiagaraStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* NiagaraCrowl;


	UPROPERTY() ABoom* BoomActor;
	UPROPERTY() ATower* Tower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Speed;

	UPROPERTY() float Damage;
	UPROPERTY() float LifeTime;
	UPROPERTY() float DamageRadius;
	UPROPERTY() float DamageTime;
	UPROPERTY() float ExtRadius;
	UPROPERTY() float StartLenght;
	

	UPROPERTY() TArray<UPrimitiveComponent*> ShieldComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> OverlappedComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> DamagedComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> TempComponents;

	UPROPERTY() FTimerHandle StartTimer;
	UPROPERTY() FTimerHandle MoveTimer;
	UPROPERTY() FTimerHandle DamageTimer;
	UPROPERTY() FTimerHandle LifeTimer;

	UFUNCTION() void CrawlFunc();
	UFUNCTION() void AreaFunc();

	UFUNCTION() void ExtOnOverlapBegin0(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void IntOnOverlapBegin0(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void IntOnOverlapBegin1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void IntOnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() void OnNiagaraSystemFinished(UNiagaraComponent* NiagaraComponent);
};


