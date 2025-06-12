// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun01.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



ATGun01::ATGun01()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun01.tgun01'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun01.MI_TGun01'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile01.BP_tProjectile01_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT01.NI_ShootT01'")));
	Niagara->SetAutoActivate(false);


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Accurary = 0.0f;
}





void ATGun01::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}





void ATGun01::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}





void ATGun01::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		Super::Fire();
		if (IsValid(Prj) && IsValid(AimComponent))
		{
			Prj->Component = AimComponent;
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}





void ATGun01::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG01_EnergyPrice);
		GunRadius->SetCapsuleRadius(GInstance->TG01_Radius);
		Gun_Delay = GInstance->TG01_GunDelay;
		SpawnDelay = GInstance->TG01_SpawnDelay;
		ShootEnergyPrice = GInstance->TG01_ShootEnergyPrice;
	}
	GInstance = nullptr;
}
