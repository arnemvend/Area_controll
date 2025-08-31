// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun11.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"


ATGun11::ATGun11()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun11.tgun11'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun11.MI_TGun11'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile11.BP_tProjectile11_C"));

	/*Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT11.NI_ShootT11'")));
	Niagara->SetAutoActivate(false);*/

	
	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Accurary = 0.0f;
}





void ATGun11::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}




void ATGun11::Tracking()
{
	Super::Tracking();

	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}




void ATGun11::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		FRotator Rotate = UKismetMathLibrary::FindLookAtRotation
		(Niagara->GetComponentLocation(), AimComponent->GetComponentLocation());
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Rotate);
		if (IsValid(Prj))
		{
			Prj->Component = AimComponent;
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}



void ATGun11::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG11_EnergyPrice);
		GunRadius->SetCapsuleRadius(GInstance->TG11_Radius);
		Gun_Delay = GInstance->TG11_GunDelay;
		SpawnDelay = GInstance->TG11_SpawnDelay;
		ShootEnergyPrice = GInstance->TG11_ShootEnergyPrice;
	}
	GInstance = nullptr;
}




