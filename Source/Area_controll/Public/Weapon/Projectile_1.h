// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_1.generated.h"


class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class ABoom;
class UProjectileMovementComponent;


UCLASS()
class AREA_CONTROLL_API AProjectile_1 : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile_1();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") TArray<USphereComponent*> SphereArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraClasterBomb;
	UPROPERTY() UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") TArray<UNiagaraComponent*> NiagaraArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileMovement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;

	UPROPERTY() float Damage;
	UPROPERTY() float Splash;
	UPROPERTY() bool CanBoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABoom> Spowned;

	UFUNCTION() void PostReact(USphereComponent* Sphere, UNiagaraComponent* Niagara, FVector Loc);
	UFUNCTION()
	void React(AActor* OtherActor, UPrimitiveComponent* OtherComp, USphereComponent* Sphere, UNiagaraComponent* Niagara, FVector Loc);

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin0(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin3(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin4(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};