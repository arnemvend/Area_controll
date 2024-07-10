// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"


class UStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;
class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UBillboardComponent;
class ATower;
class UWidgetComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class AREA_CONTROLL_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATower();


	virtual void Tick(float DeltaTime) override;


	
	UPROPERTY() UNiagaraSystem* NiagaraNetSystem;
	UPROPERTY() UStaticMesh* MyStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UCurveFloat* CreateShield;
	FOnTimelineFloat TlCallbackCreateShield;
	FOnTimelineEvent TLFinish;




protected:

	virtual void BeginPlay() override;



	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* DiskMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* PartShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* SphereShieldMesh;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* InShieldMesh;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* PartShieldInMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleInternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleExternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CollisionEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* TowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* DiskMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* PartShieldMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* SphereShieldMaterial;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* InShieldMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInstanceDynamic* DTowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInstanceDynamic* DDiskMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInstanceDynamic* DPartShieldMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInstanceDynamic* DSphereShieldMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraNet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget") UWidgetComponent* TowerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* ShieldTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* PShieldTimeLine;




	//"Other variables"----------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsRepeater;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsProcessRun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsClicked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables") bool CanDie;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Variables") int MassEnergy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int Wave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int MaxWave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int NotNet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") int EnergyPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<int> AdressTower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") float Influence;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float UpBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float DownBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float RightBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float LeftBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float EnergyStep;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<ATower*> StepTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<ATower*> NextTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<ATower*> PSNearestTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<ATower*> ChildTowers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") ATower* Main;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") ATower* MainEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") ATower* TemporaryTower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName InCollName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName ExCollName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName NameMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName NameMainEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor YourColorGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor EnemyColorGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor DefaultColor;
	
	




	//"Logical functions for Energy"------------------------------------------------------------------------------------>
	UFUNCTION() void CheckEnergy();//Step of area calculating
	UFUNCTION() void CheckNew(ATower* Tower);
	UFUNCTION() void ReloadEnergy(ATower* Tower, FName DName);



	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void IsMainFunc();
	UFUNCTION() void ReEnter(FName AName);
	UFUNCTION() void NetOff();
	UFUNCTION() void MainFinder(ATower* TTower);
	UFUNCTION() void ReFinder(FName BName);
	UFUNCTION(BlueprintCallable) void TowerDestroy();
	UFUNCTION() void ColorsFunc(FColor Color);
	UFUNCTION() void CheckStep(int MWave);
	UFUNCTION() void SetParam();
	UFUNCTION(BlueprintCallable) void Repeater();



	//"Logical functions for touch"------------------------------------------------------------------------------------->
	UFUNCTION(BlueprintCallable) void DeTouch();
	UFUNCTION(BlueprintCallable) void Touch();



	//"Logical functions for shield"------------------------------------------------------------------------------------>
	UFUNCTION(BlueprintCallable) void PartShieldCreate(FVector InitLoc, bool Up);
	UFUNCTION() void PartAmountFunc(float Value);
	UFUNCTION() void PShieldOn();
	UFUNCTION() void ShieldOn();
	UFUNCTION() void PartShieldProcessOff();
	UFUNCTION(BlueprintCallable) void PartCommandFunc(bool Up);
	UFUNCTION(BlueprintCallable) void ShieldCreate();
	UFUNCTION() void AmountFunc(float Value);
	UFUNCTION() void ShieldProcessOff();
	UFUNCTION() void ScaleFunc(UStaticMeshComponent* Mesh);
};
