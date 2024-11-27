// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


class UNiagaraComponent;
class UNiagaraSystem;
class UStaticMeshComponent;
class UStaticMesh;
class UCapsuleComponent;
class UMaterialInterface;
class UTimelineComponent;
class UCurveFloat;



UCLASS()
class AREA_CONTROLL_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY() int Type;

	UFUNCTION() void Start();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialInterface* GunMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCapsuleComponent* GunRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTimelineComponent* RotateTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves") UCurveFloat* CurveFloat;

	UPROPERTY() FOnTimelineFloat TLCallback;
	UPROPERTY() FOnTimelineEvent TLFinish;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Gun_Speed;
	UPROPERTY() bool CanStartTimer;
	UPROPERTY() FVector AimLoc;
	UPROPERTY() TArray<AActor*> Aims;
	UPROPERTY() AActor* AimActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<AActor> SpownProjectile;
	UPROPERTY() FRotator Rot;
	UPROPERTY() FTimerHandle TimerAim;
	UPROPERTY() FTimerHandle TimerFire;

	
	UFUNCTION() void Find();
	UFUNCTION() void Rotate(float Amount);
	UFUNCTION() void FireLogic();
	UFUNCTION() void Fire();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
