// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Wild.generated.h"


class AGun;
class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UTimelineComponent;
class UCurveFloat;
class UBoxComponent;
class UChildActorComponent;
class USplineComponent;
class UNiagaraSystem;
class ABoom;
class APreloadActor;




UCLASS()
class AREA_CONTROLL_API AWild : public AActor
{
	GENERATED_BODY()
	
public:	

	AWild();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY() FVector AimCoord;
	UPROPERTY() int Type;

	UFUNCTION() void Start();

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* MeshShadow;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY() UMaterialInterface* MeshMaterial;
	UPROPERTY() UMaterialInterface* ShadowMaterial;

	UPROPERTY() UMaterialInstanceDynamic* DMeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* Box;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* Nose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UChildActorComponent* Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTimelineComponent* StartTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTimelineComponent* MoveTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves") UCurveFloat* CurveFloat0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curves") UCurveFloat* CurveFloat1;

	UPROPERTY() FOnTimelineFloat TLCallbackStart;
	UPROPERTY() FOnTimelineFloat TLCallbackMove;
	UPROPERTY() FOnTimelineFloat TLCallbackHigh;
	UPROPERTY() FOnTimelineEvent TLFinish;

	UPROPERTY() UNiagaraSystem* NiagaraSystem;

	UPROPERTY() ABoom* BoomActor;
	UPROPERTY() APreloadActor* PActor;

	
	UPROPERTY() FVector CurrentCoord;
	UPROPERTY() FVector CurrentScaleShMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float EndScaleShMesh;
	UPROPERTY() float Speed;
	UPROPERTY() float High;
	UPROPERTY() float Health;
	UPROPERTY() float MaxHealth;
	UPROPERTY() FRotator CurrentRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABoom> Spowned;


	
	UFUNCTION() void CreateFunc(float Amount);
	UFUNCTION() void Continue();
	UFUNCTION() void MoveFunc(float Amount);
	UFUNCTION() void HighFunc(float Amount);
	UFUNCTION() void FinishFunc();
	UFUNCTION() void DestroyFunc();

	UFUNCTION()
	void OnBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
	void OnBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnNoseOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
