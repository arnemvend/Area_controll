// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile11.generated.h"


class UNiagaraSystem;
class ABoom;
class USphereComponent;
class UNiagaraComponent;
class USceneComponent;




UCLASS()
class AREA_CONTROLL_API AtProjectile11 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile11();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraSystem* NSystem;

	UPROPERTY() TArray<UNiagaraComponent*> NiagaraArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* MainSphere;
	UPROPERTY() TArray<USphereComponent*> SphereArr;
	//UPROPERTY() TArray<USceneComponent*> PointArr;
	UPROPERTY() USceneComponent* Point;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;

	UPROPERTY() float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Speed;
	UPROPERTY() float Expansion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DestroyTime;
	UPROPERTY() int RocketNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float RotSpeed;

	UPROPERTY() bool CanDestroy;
	UPROPERTY() bool CanRot;
	UPROPERTY() TArray<FVector> Loc;
	//UPROPERTY() TArray<float> RotAngles;

	UFUNCTION() void DeleteFunc(int a);
	UFUNCTION() void EndOfWay();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
