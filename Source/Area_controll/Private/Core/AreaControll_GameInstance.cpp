// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/AreaControll_GameInstance.h"
#include "Weapon/Gun.h"


UAreaControll_GameInstance::UAreaControll_GameInstance()
{
	TGuns.SetNum(12);
	OpenTGuns.SetNum(12);

	//TGunsTable for spawn logic
	TGuns[0] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun00.BP_TGun00_C"));
	TGuns[1] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun01.BP_TGun01_C"));
	TGuns[2] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun02.BP_TGun02_C"));
	TGuns[3] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun03.BP_TGun03_C"));
	TGuns[4] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun10.BP_TGun10_C"));
	TGuns[5] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun11.BP_TGun11_C"));
	TGuns[6] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun12.BP_TGun12_C"));
	TGuns[7] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun13.BP_TGun13_C"));
	TGuns[8] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun20.BP_TGun20_C"));
	TGuns[9] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun21.BP_TGun21_C"));
	TGuns[10] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun22.BP_TGun22_C"));
	TGuns[11] = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_TGun23.BP_TGun23_C"));

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
	//TGun02
	TG02_GunDelay = 0.25f;
	TG02_Radius = 300.0f;
	TG02_Damage = 2.0f;
	TG02_SpawnDelay = 2.5f;
	TG02_Accurary = 8.0f;
	TG02_EnergyPrice = 40;
	TG02_ShootEnergyPrice = 2;
	//TGun03
	TG03_GunDelay = 0.8f;
	TG03_Radius = 350.0f;
	TG03_Damage = 2.0f;
	TG03_SpawnDelay = 5.0f;
	TG03_EnergyPrice = 100;
	TG03_ShootEnergyPrice = 5;
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
	//TGun12
	TG12_GunDelay = 1.0f;
	TG12_Radius = 350.0f;
	TG12_Damage = 3.0f;
	TG12_SpawnDelay = 4.0f;
	TG12_EnergyPrice = 80;
	TG12_ShootEnergyPrice = 8;
	//TGun13
	TG13_GunDelay = 3.0f;
	TG13_Radius = 350.0f;
	TG13_DownSpeed = -0.2f;
	TG13_DownTime = 2.5f;
	TG13_Damage = 0.0f;
	TG13_SpawnDelay = 5.0f;
	TG13_EnergyPrice = 200;
	TG13_ShootEnergyPrice = 10;
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
	//TGun22
	TG22_GunDelay = 3.0f;
	TG22_Radius = 500.0f;
	TG22_ExtRadius = 300.0f;
	TG22_Accurary = 70.0f;
	TG22_Damage = 20.0f;
	TG22_SpawnDelay = 8.0f;
	TG22_EnergyPrice = 300;
	TG22_ShootEnergyPrice = 100;
	//TGun23
	TG23_GunDelay = 25.0f;
	TG23_Radius = 700.0f;
	TG23_ExtRadius = 200.0f;
	TG23_Accurary = 0.0f;
	TG23_Damage = 10.0f;
	TG23_SpawnDelay = 8.0f;
	TG23_LifeTime = 30.0f;
	TG23_DamageRadius = 250.0f;
	TG23_DamageTime = 1.0f;
	TG23_EnergyPrice = 300;
	TG23_ShootEnergyPrice = 300;


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
	//Wild2
	W2_Speed = 18.0f;
	W2_High = 200.0f;
	W2_MaxHealth = 30.0f;
	G2_GunDelay = 10.0f;
	G2_Radius = 450.0f;
	G2_Damage = 60.0f;
	G2_PrHealth = 10.0f;
	G2_PrSpeed = 2.5f;
}





float UAreaControll_GameInstance::GunProperty(int Type, int Number, const FString& PropertyName)
{
	FString FullName = FString::Printf(TEXT("TG%d%d_%s"), Type, Number, *PropertyName);

	FProperty* Property = GetClass()->FindPropertyByName(FName(*FullName));

	if (Property)
	{
		float* Value = Property->ContainerPtrToValuePtr<float>(this);
		if (Value)
		{
			return *Value;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


