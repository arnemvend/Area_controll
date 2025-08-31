// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun12.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"


ATGun12::ATGun12()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun12.tgun12'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun12.MI_TGun12'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile12.BP_tProjectile12_C"));

	


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Accurary = 0.0f;
}





void ATGun12::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG12_EnergyPrice);
		GunRadius->SetCapsuleRadius(GInstance->TG12_Radius);
		Gun_Delay = GInstance->TG12_GunDelay;
		SpawnDelay = GInstance->TG12_SpawnDelay;
		ShootEnergyPrice = GInstance->TG12_ShootEnergyPrice;
	}
	GInstance = nullptr;
}





void ATGun12::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}





void ATGun12::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}





void ATGun12::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Niagara->GetComponentRotation());
		if (IsValid(Prj) && IsValid(AimComponent))
		{
			Prj->Component = AimComponent;
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}



