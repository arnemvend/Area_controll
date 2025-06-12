// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AreaControll_GameInstance.generated.h"

class AGun;

UCLASS()
class AREA_CONTROLL_API UAreaControll_GameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	UAreaControll_GameInstance();

	//Colors----------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor DefaultColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor YourColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor EnemyColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor WildColor;

	//TowerGuns Array-------------------------------------------->
	//for TowerWidget logic
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TowerGun Table") TArray<TSubclassOf<AGun>> TGuns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TowerGun Table") TArray<bool> OpenTGuns;

	//Construction----------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction") float Cn_StartHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction") float Cn_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction") float Cn_AddHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction") int Cn_EnergyPrice;

	//EnergyTower------------------------------------------------>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower") float ShieldEnergyLoss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower") float Repear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower") float EnergyRepear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|EnergyTower") float Tr_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|MainTower") float MTr_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|MainTower") int MTr_MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|MainTower") float MTr_EnergyStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|MainTower") int MTr_EnergyPoint;

	//TGuns------------------------------------------------------>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") float TG00_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") float TG00_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") float TG00_ExtRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") float TG00_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") float TG00_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") int TG00_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun00") int TG00_ShootEnergyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") float TG01_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") float TG01_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") float TG01_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") float TG01_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") float TG01_JumpRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") int TG01_MaxJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") int TG01_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun01") int TG01_ShootEnergyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") float TG10_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") float TG10_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") float TG10_ExtRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") float TG10_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") float TG10_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") int TG10_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun10") int TG10_ShootEnergyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") float TG11_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") float TG11_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") float TG11_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") float TG11_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") float TG11_Expansion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") int TG11_RocketNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") int TG11_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun11") int TG11_ShootEnergyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_ExtRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_ExplRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") float TG20_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") int TG20_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun20") int TG20_ShootEnergyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_ExtRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_TimeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_LenghtExpl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") float TG21_SpawnDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") int TG21_EnergyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TGuns|TGun21") int TG21_ShootEnergyPrice;

	//WildFabric------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WildFabric") float WF_MaxHealth;

	//Lighters--------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0") float W0_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0") float W0_High;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0") float W0_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0|Gun0") float G0_Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0|Gun0") float G0_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0|Gun0") float G0_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild0|Gun0") float G0_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1") float W1_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1") float W1_High;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1") float W1_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1|Gun1") float G1_Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1|Gun1") float G1_GunDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1|Gun1") float G1_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1|Gun1") float G1_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighters|Wild1|Gun1") float G1_Splash;



protected:
};
