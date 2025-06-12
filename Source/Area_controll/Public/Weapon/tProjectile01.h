// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "tProjectile01.generated.h"



class USphereComponent;
class UNiagaraComponent;
class ABoom;



UCLASS()
class AREA_CONTROLL_API AtProjectile01 : public AProjectile
{
	GENERATED_BODY()

public:

	AtProjectile01();

	virtual void Tick(float DeltaTime) override;


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY() TArray<UPrimitiveComponent*> OverlappedComponents;
	

	UPROPERTY() float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Speed;
	UPROPERTY() float JumpRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float DeathDelay;
	UPROPERTY() float RealyScale;
	UPROPERTY() int MaxJump;
	UPROPERTY() int Jump;


	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() FTimerHandle Timer;
	UPROPERTY() FTimerHandle DestroyTimer;



	UFUNCTION() void JumpFunc();
	UFUNCTION() void DeathFunc();
	UFUNCTION() void CallBoom(FVector Loc);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
