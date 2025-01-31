// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"


class ABoom;
class AMainTower;
class APreloadActor;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UTimelineComponent;
class UCurveFloat;
class UTowerWidget;


UCLASS()
class AREA_CONTROLL_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATower();


	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* SphereShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") UTowerWidget* TW;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float MaxHealth;
	UPROPERTY() float CurrentEmissive;

	UPROPERTY() int Wave;
	UPROPERTY() TArray<int> AdressTower;

	UPROPERTY() bool IsRepeater;
	UPROPERTY() bool IsProcessRun;
	UPROPERTY() bool IsClicked;




	UFUNCTION() void Start(bool IsYour);
	UFUNCTION() void SetDamageFunc();
	UFUNCTION() void ShieldCreate();
	UFUNCTION() void TowerDestroy();

	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void NetOff();
	UFUNCTION() void ReEnter(FName AName);
	UFUNCTION() void Repeater();
	UFUNCTION() void DeTouch();
	


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* DiskMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* PartShieldMesh;
	
	UPROPERTY() UStaticMesh* MyStaticMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleInternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleExternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CollisionEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* TowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* DiskMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* PartShieldMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* SphereShieldMaterial;

	UPROPERTY() UMaterialInstanceDynamic* DTowMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DDiskMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DPartShieldMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DSphereShieldMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraNet;
	UPROPERTY() UNiagaraComponent* NiagaraRepeater;
	UPROPERTY() UNiagaraSystem* NiagaraNetSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* ShieldTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* PShieldTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UCurveFloat* CreateShield;
	UPROPERTY() FOnTimelineFloat TlCallbackCreateShield;
	UPROPERTY() FOnTimelineEvent TLFinish;

	UPROPERTY() ABoom* BoomActor;
	UPROPERTY() APreloadActor* PActor;



	//"Other variables"----------------------------------------------------------------------------------------->
	
	UPROPERTY() bool CanDie;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int MaxWave;
	UPROPERTY() int NotNet;

	UPROPERTY() float Influence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float BoomDuration;

	UPROPERTY() TArray<ATower*> StepTowers;
	UPROPERTY() TArray<ATower*> NextTowers;
	UPROPERTY() TArray<ATower*> PSNearestTowers;
	UPROPERTY() TArray<ATower*> ChildTowers;

	UPROPERTY() AMainTower* Main;
	UPROPERTY() AMainTower* MainEnemy;
	UPROPERTY() ATower* TemporaryTower;

	UPROPERTY() FName Name;
	UPROPERTY() FName InCollName;
	UPROPERTY() FName ExCollName;
	UPROPERTY() FName NameMain;
	UPROPERTY() FName NameMainEnemy;

	UPROPERTY() FTimerHandle Timer0;
	//UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;
	UPROPERTY() FTimerHandle Timer3;
	UPROPERTY() FTimerHandle Timer4;

	UPROPERTY() FColor MyColor;


	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		AController* InstigatedBy, AActor* DamageCauser);



	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void IsMainFunc(bool IsYour);
	UFUNCTION() void ColorsFunc(FColor Color);
	UFUNCTION() void CheckStep(int MWave);
	UFUNCTION() void SetParam();
	



	//"Logical functions for touch"------------------------------------------------------------------------------------->
	
	UFUNCTION() void Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);



	//"Logical functions for shield"------------------------------------------------------------------------------------>
	UFUNCTION() void PartShieldCreate(FVector InitLoc, bool Up);
	UFUNCTION() void PartAmountFunc(float Value);
	UFUNCTION() void PShieldOn();
	UFUNCTION() void ShieldOn();
	UFUNCTION() void PartShieldProcessOff();
	UFUNCTION() void PartCommandFunc(bool Up);
	
	UFUNCTION() void AmountFunc(float Value);
	UFUNCTION() void ShieldProcessOff();
	UFUNCTION() void ScaleFunc(UStaticMeshComponent* Mesh);
	
};
