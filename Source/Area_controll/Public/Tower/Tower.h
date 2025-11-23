// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"


class ABoom;
class AMainTower;
class UChildActorComponent;
class UAreaControll_GameInstance;
class AAreaControll_GameMode;
class AAreaControll_PlayerController;
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
//class UGameWidget;



UCLASS()
class AREA_CONTROLL_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATower();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* SphereShieldMesh;

	UPROPERTY() UTowerWidget* TW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns") UChildActorComponent* UpGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns") UChildActorComponent* MidGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns") UChildActorComponent* LowGun;

	UPROPERTY() float Health;
	UPROPERTY() float MaxHealth;
	UPROPERTY() float CurrentEmissive;

	UPROPERTY() int Wave;
	UPROPERTY() int GunNum;
	UPROPERTY() TArray<int> AdressTower;

	UPROPERTY() bool IsRepeater;
	UPROPERTY() bool IsProcessRun;
	UPROPERTY() bool IsClicked;
	UPROPERTY() bool CanDamage;
	UPROPERTY() bool IsAuto;
	UPROPERTY() bool HaveLowGun;



	UFUNCTION() void ShieldCreate();
	UFUNCTION() void Start(bool IsYour);
	UFUNCTION() void SetDamageFunc();
	UFUNCTION() void RunDelete();
	UFUNCTION() void DeTouch(int Value);

	UFUNCTION() void CreateGun(int Type, int Number);
	UFUNCTION() void DeleteGun(int Type);
	UFUNCTION() void SetAutoFire(bool AutoFire);
	UFUNCTION() void SetAimComponents();
	UFUNCTION() void UnSetAimComponents();
	UFUNCTION() void TargetAim(UPrimitiveComponent* Component);
	UFUNCTION() void OnOffTargetEffect(bool On);


	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void NetOff();
	UFUNCTION() void ReEnter(FName AName);
	UFUNCTION() void Repeater();
	
	


protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* DiskMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* PartShieldMesh;
	
	UPROPERTY() UStaticMesh* MyStaticMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger") UCapsuleComponent* TriggerCapsuleInternal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger") UCapsuleComponent* TriggerCapsuleExternal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger") UCapsuleComponent* CollisionEnergy;

	UPROPERTY() UMaterialInterface* TowMaterial;
	UPROPERTY() UMaterialInterface* DiskMaterial;
	UPROPERTY() UMaterialInterface* PartShieldMaterial;
	UPROPERTY() UMaterialInterface* SphereShieldMaterial;

	UPROPERTY() UMaterialInstanceDynamic* DTowMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DDiskMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DPartShieldMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DSphereShieldMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraNet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraRepeater;
	UPROPERTY() UNiagaraSystem* NiagaraNetSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* ShieldTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* PShieldTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UCurveFloat* CreateShield;
	UPROPERTY() FOnTimelineFloat TlCallbackCreateShield;
	UPROPERTY() FOnTimelineEvent TLFinish;

	UPROPERTY() ABoom* BoomActor;
	UPROPERTY() UAreaControll_GameInstance* GInstance;
	UPROPERTY() AAreaControll_GameMode* GMode;
	UPROPERTY() AAreaControll_PlayerController* PController;



	//"Other variables"----------------------------------------------------------------------------------------->
	
	UPROPERTY() bool CanDie;

	UPROPERTY() int MaxWave;
	UPROPERTY() int EnergyLoss;

	UPROPERTY() float Influence;
	UPROPERTY() float Repear;
	UPROPERTY() int EnergyRepear;

	UPROPERTY() TArray<ATower*> StepTowers;
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

	UPROPERTY() FColor MyColor;

	UPROPERTY() FTimerHandle Timer010;
	UPROPERTY() FTimerHandle Timer011;
	UPROPERTY() FTimerHandle Timer012;
	UPROPERTY() FTimerHandle Timer013;
	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;
	UPROPERTY() FTimerHandle Timer3;
	//UPROPERTY() FTimerHandle Timer4;




	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		AController* InstigatedBy, AActor* DamageCauser);



	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void IsMainFunc(bool IsYour);
	UFUNCTION() void ColorsFunc(FColor Color);
	UFUNCTION() void CheckStep(int MWave);
	UFUNCTION() void SetParam();
	UFUNCTION() void TowerDestroy();
	UFUNCTION() void TowerDelete();
	



	//"Logical functions for touch"------------------------------------------------------------------------------------->
	
	UFUNCTION() void Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);



	//"Logical functions for shield"------------------------------------------------------------------------------------>
	UFUNCTION() void PartShieldCreate(FVector InitLoc, bool Up);
	UFUNCTION() void PartAmountFunc(float Value);
	UFUNCTION() void PShieldOn();
	UFUNCTION() void ShieldOn();
	UFUNCTION() void PartShieldProcessOff();
	UFUNCTION() void PartCommandFunc(bool Up);
	UFUNCTION() void CanFire(bool CanFire, bool bNeedStop, bool bNeedStart);

	UFUNCTION() void AmountFunc(float Value);
	UFUNCTION() void ShieldProcessOff();
	UFUNCTION() void ScaleFunc(UStaticMeshComponent* Mesh);
	UFUNCTION() void CheckShieldansRun(bool IsPlayer, bool ShieldIsOn);

	
};
