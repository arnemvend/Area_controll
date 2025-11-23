// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile12.generated.h"


class USphereComponent;
class UNiagaraComponent;
class ABoom;



UCLASS()
class AREA_CONTROLL_API AtProjectile12 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile12();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara") UNiagaraComponent* Niagara;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float CustomDTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Boost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float DeathDelay;

	UPROPERTY() float Damage;
	UPROPERTY() float StartTime;
	UPROPERTY() float CurrentTime;
	UPROPERTY() FVector StartOffset;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FTimerHandle Timer;
	UPROPERTY() FTimerHandle DestroyTimer;

	

	UFUNCTION() void DeathFunc();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
