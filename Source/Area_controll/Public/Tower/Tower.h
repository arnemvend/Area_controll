// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"


class ABoom;
class AMainTower;
class UStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;
class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor YourColorGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor EnemyColorGround;


	UFUNCTION() void SetDamageFunc();


	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void NetOff();
	UFUNCTION() void ReEnter(FName AName);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int Wave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") TArray<int> AdressTower;


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* DiskMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* PartShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* SphereShieldMesh;
	UPROPERTY() UStaticMesh* MyStaticMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleInternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsuleExternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CollisionEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* TowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* DiskMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* PartShieldMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* SphereShieldMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DTowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DDiskMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DPartShieldMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DSphereShieldMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraNet;
	UPROPERTY() UNiagaraSystem* NiagaraNetSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget") UWidgetComponent* TowerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* ShieldTimeLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UTimelineComponent* PShieldTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline") UCurveFloat* CreateShield;
	UPROPERTY() FOnTimelineFloat TlCallbackCreateShield;
	UPROPERTY() FOnTimelineEvent TLFinish;



	//"Other variables"----------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsRepeater;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsProcessRun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") bool IsClicked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables") bool CanDie;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int MaxWave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") int NotNet;
	

	

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") float Influence;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") TArray<ATower*> StepTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") TArray<ATower*> NextTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") TArray<ATower*> PSNearestTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") TArray<ATower*> ChildTowers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") AMainTower* Main;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") AMainTower* MainEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References") ATower* TemporaryTower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABoom> SpownBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName InCollName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName ExCollName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName NameMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FName NameMainEnemy;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FColor DefaultColor;

	UPROPERTY() FTimerHandle Timer0;
	//UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;
	UPROPERTY() FTimerHandle Timer3;
	
	


	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		AController* InstigatedBy, AActor* DamageCauser);


	



	//"Logical functions for network search"---------------------------------------------------------------------------->
	UFUNCTION() void IsMainFunc();
	
	UFUNCTION(BlueprintCallable) void TowerDestroy();
	UFUNCTION() void ColorsFunc(FColor Color);
	UFUNCTION() void CheckStep(int MWave);
	UFUNCTION() void SetParam();
	UFUNCTION(BlueprintCallable) void Repeater();



	//"Logical functions for touch"------------------------------------------------------------------------------------->
	UFUNCTION(BlueprintCallable) void DeTouch();
	UFUNCTION(BlueprintCallable) void Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);



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
