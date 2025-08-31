// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wild.generated.h"


class AGun;
class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UCurveFloat;
class UBoxComponent;
class UChildActorComponent;
class USplineComponent;
class UNiagaraSystem;
class ABoom;





UCLASS()
class AREA_CONTROLL_API AWild : public AActor
{
	GENERATED_BODY()
	
public:	

	AWild();

	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* Box;

	UPROPERTY() FVector AimCoord;
	UPROPERTY() FColor Color;

	UPROPERTY() float DownDpeed;
	UPROPERTY() float DownTime;

	UFUNCTION() void Start();
	//for tgun13 functions
	UFUNCTION() void MoveDownStart();
	UFUNCTION() void MoveDownStop();

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* MeshShadow;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY() UMaterialInterface* MeshMaterial;

	UPROPERTY() UMaterialInstanceDynamic* DMeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* Nose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UChildActorComponent* Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves") UCurveFloat* CurveFloat1;

	UPROPERTY() UNiagaraSystem* NiagaraSystem;

	UPROPERTY() FTimerHandle CreateTimer;
	UPROPERTY() FTimerHandle MoveTimer;
	UPROPERTY() FTimerHandle DownTimer;
	UPROPERTY() FTimerHandle DownStop;

	UPROPERTY() ABoom* BoomActor;

	UPROPERTY() int CurrentMoveNumber;
	UPROPERTY() int EndUp;
	UPROPERTY() float Speed;
	UPROPERTY() float High;
	UPROPERTY() float Health;
	UPROPERTY() float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CreationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CustomDeltaTime;
	UPROPERTY() float CurrentCreateValue;
	UPROPERTY() bool CanDamage;
	UPROPERTY() bool CanDown;

	UPROPERTY() TArray<FVector> LocArr;
	UPROPERTY() TArray<FRotator> RotArr;



	UFUNCTION() void Continue();

	UFUNCTION()
	void OnBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
	void OnBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnNoseOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
};
