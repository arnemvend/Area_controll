// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun22.h"
#include "Weapon/tProjectile22.h"
#include "Components/CapsuleComponent.h"
//#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
//#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"


ATGun22::ATGun22()
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun22.tgun22'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun22.MI_TGun22'"));

	GunMesh->SetMaterial(0, GunMaterial);

	GunRadius->ComponentTags.Add("Radius");

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile22.BP_tProjectile22_C"));

	/*Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT20.NI_ShootT20'")));
	Niagara->SetAutoActivate(false);*/

	EnemyNames.Empty();
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");
}





void ATGun22::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG22_EnergyPrice);
		ExtRadius = GInstance->TG22_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG22_Radius);
		Gun_Delay = GInstance->TG22_GunDelay;
		SpawnDelay = GInstance->TG22_SpawnDelay;
		ShootEnergyPrice = GInstance->TG22_ShootEnergyPrice;
	}
	GInstance = nullptr;
}





void ATGun22::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice && IsValid(AimComponent))
	{
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Niagara->GetComponentRotation());

		if (IsValid(Prj))
		{
			Prj->Aim = FVector(AimComponent->GetComponentLocation());
			AtProjectile22* Projectile = Cast<AtProjectile22>(Prj);
			if (IsValid(Projectile))
			{
				Projectile->Start();
			}
			Projectile = nullptr;
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
		Prj = nullptr;
	}
}




