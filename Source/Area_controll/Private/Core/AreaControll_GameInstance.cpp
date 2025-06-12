// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/AreaControll_GameInstance.h"
#include "Weapon/Gun.h"


UAreaControll_GameInstance::UAreaControll_GameInstance()
{
	TGuns.SetNum(15);
	OpenTGuns.SetNum(15);

	//TGunsTable for spawn logic
	TGuns[0] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun00.BP_TGun00_C"));
	TGuns[1] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun01.BP_TGun01_C"));
	TGuns[5] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun10.BP_TGun10_C"));
	TGuns[6] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun11.BP_TGun11_C"));
	TGuns[10] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun20.BP_TGun20_C"));
	TGuns[11] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun21.BP_TGun21_C"));

	//ConstructionActor variables
	Cn_StartHealth = 10.0f;
	Cn_MaxHealth = 30.0f;
	Cn_AddHealth = 2.0f;
	Cn_EnergyPrice = 100;

	//EnergyTower
	ShieldEnergyLoss = 50;
	Repear = 3.0f;
	EnergyRepear = 5;
	Tr_MaxHealth = 150.0f;
	MTr_MaxHealth = 300.0f;
	MTr_MaxEnergy = 3000;
	MTr_EnergyStep = 20.0f;
	MTr_EnergyPoint = 1;

	//TGuns------------------------------------>
	//TGun00
	TG00_GunDelay = 1.0f;
	TG00_Radius = 400.0f;
	TG00_ExtRadius = 200.0f;
	TG00_Damage = 5.0f;
	TG00_SpawnDelay = 4.0f;
	TG00_EnergyPrice = 50;
	TG00_ShootEnergyPrice = 5;
	//TGun01
	TG01_GunDelay = 0.8f;
	TG01_Radius = 280.0f;
	TG01_Damage = 3.0f;
	TG01_MaxJump = 5;
	TG01_JumpRadius = 150.0f;
	TG01_SpawnDelay = 4.0f;
	TG01_EnergyPrice = 80;
	TG01_ShootEnergyPrice = 8;
	//TGun10
	TG10_GunDelay = 1.3f;
	TG10_Radius = 450.0f;
	TG10_ExtRadius = 250.0f;
	TG10_Damage = 5.0f;
	TG10_SpawnDelay = 6.0f;
	TG10_EnergyPrice = 120;
	TG10_ShootEnergyPrice = 15;
	//TGun11
	TG11_GunDelay = 2.0f;
	TG11_Radius = 370.0f;
	TG11_Damage = 10.0f;
	TG11_SpawnDelay = 8.0f;
	TG11_Expansion = 1.2f;
	TG11_RocketNum = 6;
	TG11_EnergyPrice = 90;
	TG11_ShootEnergyPrice = 20;
	//TGun20
	TG20_GunDelay = 5.0f;
	TG20_Radius = 600.0f;
	TG20_ExtRadius = 350.0f;
	TG20_Accurary = 120.0f;
	TG20_Damage = 80.0f;
	TG20_ExplRadius = 90.0f;
	TG20_SpawnDelay = 10.0f;
	TG20_EnergyPrice = 500;
	TG20_ShootEnergyPrice = 200;
	//TGun21
	TG21_GunDelay = 5.0f;
	TG21_Radius = 500.0f;
	TG21_ExtRadius = 250.0f;
	TG21_Damage = 10.0f;
	TG21_TimeStep = 0.5f;
	TG21_LenghtExpl = 150.0f;
	TG21_SpawnDelay = 9.0f;
	TG21_EnergyPrice = 150;
	TG21_ShootEnergyPrice = 120;


	//WildFabric
	WF_MaxHealth = 300.0f;

	//Lighters---------------------------------------->
	//Wild0
	W0_Speed = 25.0f;
	W0_High = 100.0f;
	W0_MaxHealth = 20.0f;
	G0_Accurary = 12.0f;
	G0_GunDelay = 0.25f;
	G0_Radius = 300.0f;
	G0_Damage = 2.0f;
	//Wild1
	W1_Speed = 20.0f;
	W1_High = 200.0f;
	W1_MaxHealth = 50.0f;
	G1_GunDelay = 3.0f;
	G1_Radius = 350.0f;
	G1_Damage = 10.0f;
	G1_Splash = 25.0f;
}


