// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Structure/StructWild.h"
#include "NiagaraSystem.h"
#include "Weapon/Projectile_0.h"


FStructWild::FStructWild()
{
	//UNIT 0 ----------------------------------------------------->
	Health.Add(10.0f);
	Speed.Add(25.0f);
	High.Add(200.0f);
	SM_Main.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild0.Wild0'")));
	SM_Light.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild0_light.Wild0_light'")));
	MI_Main.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Wild0.MI_Wild0'")));
	MI_Light.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild0_light.MI_wild0_light'")));
	MI_Shadow.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Shadow_Light0.MI_Shadow_Light0'")));
	NS_Boom.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight0.NI_BoomLight0'")));
	//Unit 0 Gun
	Gun_SM.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun0.WildGun0'")));
	Gun_MI.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild0gun.MI_wild0gun'")));
	Gun_NS.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot0.NI_Shoot0'")));
	Gun_Radius.Add(300.0f);
	Gun_Accurary.Add(12.0f);
	Gun_Speed.Add(0.25f);
	Gun_BP.Add(LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/BP_Projectile_0.BP_Projectile_0_C")));


	//UNIT 1 ----------------------------------------------------->
	Health.Add(20.0f);
	Speed.Add(20.0f);
	High.Add(240.0f);
	SM_Main.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild1.Wild1'")));
	SM_Light.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild1_light.Wild1_light'")));
	MI_Main.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Wild1.MI_Wild1'")));
	MI_Light.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild1_light.MI_wild1_light'")));
	MI_Shadow.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Shadow_Light1.MI_Shadow_Light1'")));
	NS_Boom.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight1.NI_BoomLight1'")));
	//Unit 1 Gun
	Gun_SM.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun1.WildGun1'")));
	Gun_MI.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild1gun.MI_wild1gun'")));
	Gun_NS.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot1.NI_Shoot1'")));
	Gun_Radius.Add(350.0f);
	Gun_Accurary.Add(0.0f);
	Gun_Speed.Add(3.0f);
	Gun_BP.Add(LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/BP_Projectile_1.BP_Projectile_1_C")));
}

