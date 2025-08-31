// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun21.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



ATGun21::ATGun21()
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun21.tgun21'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun21.MI_TGun21'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile21.BP_tProjectile21_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot1.NI_Shoot1'")));
	Niagara->SetAutoActivate(false);



	EnemyNames.Empty();
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");

	Accurary = 0.0f;
}



bool ATGun21::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 30.0) || Dist <= ExtRadius);
}



void ATGun21::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}



void ATGun21::Tracking()
{
}



void ATGun21::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Niagara->GetComponentRotation());
		if (IsValid(Prj) && IsValid(AimComponent))
		{
			Prj->Aim = AimComponent->GetComponentLocation();
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}






void ATGun21::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG21_EnergyPrice);
		ExtRadius = GInstance->TG21_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG21_Radius);
		Gun_Delay = GInstance->TG21_GunDelay;
		SpawnDelay = GInstance->TG21_SpawnDelay;
		ShootEnergyPrice = GInstance->TG21_ShootEnergyPrice;
	}
	GInstance = nullptr;
}

