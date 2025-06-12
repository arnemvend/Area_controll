// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


class UNiagaraComponent;
class UNiagaraSystem;
class UPrimitiveComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UCapsuleComponent;
class UMaterialInterface;
class UTimelineComponent;
class UCurveFloat;
class AProjectile;
class UMaterialInstanceDynamic;
class UAreaControll_GameInstance;
class AAreaControll_GameMode;




UCLASS()
class AREA_CONTROLL_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGun();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY() bool IsShieldOn;
	UPROPERTY() UMaterialInstanceDynamic* DMaterial;

	UFUNCTION() void Start();
	UFUNCTION() void ColorFunc(FColor Color);
	UFUNCTION() void SpawnFunc(FColor Color);
	UFUNCTION() void DeleteFunc();
	UFUNCTION() void TimerElapsed();
	UFUNCTION() void Stop();


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY() UAreaControll_GameInstance* GInstance;
	UPROPERTY() AAreaControll_GameMode* GMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* SpawnNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") float V2Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") float OpasityPower2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* GunMesh;

	UPROPERTY() UMaterialInterface* GunMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCapsuleComponent* GunRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTimelineComponent* RotateTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTimelineComponent* FreeRotateTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves") UCurveFloat* CurveFloat;

	UPROPERTY() FOnTimelineFloat TLCallback;
	UPROPERTY() FOnTimelineFloat TLFreeCallback;
	UPROPERTY() FOnTimelineEvent TLFinish;


	UPROPERTY() TArray<FName> EnemyNames;
	UPROPERTY() float Accurary;
	UPROPERTY() float Gun_Delay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float FreeInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float FreeAngle;
	UPROPERTY() float SpawnDelay;

	UPROPERTY() bool IsFirst;
	UPROPERTY() float CurrentEM;


	UPROPERTY() TArray<UPrimitiveComponent*> AimComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> OverlappedComponents;
	UPROPERTY() UPrimitiveComponent* AimComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<AProjectile> SpownProjectile;
	UPROPERTY() AProjectile* Prj;
	UPROPERTY() FRotator Rot;

	UPROPERTY() FTimerHandle TimerAim;
	UPROPERTY() FTimerHandle TimerFire;
	UPROPERTY() FTimerHandle TimerFreeRotate;
	UPROPERTY() FTimerHandle TimerSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName CurrentComponentTag;



	UFUNCTION() virtual void Rotate(float Amount);
	UFUNCTION() void FireLogic();
	UFUNCTION() virtual void Tracking();
	UFUNCTION() virtual void Fire();
	UFUNCTION() virtual bool ComponentIsFar(UPrimitiveComponent* Component);
	UFUNCTION() virtual float HorizontalDistance(FVector A, FVector B);
	UFUNCTION() void FreeRotate(float Amount);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

};
