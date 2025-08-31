// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun23.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Weapon/Projectile.h"
#include "Weapon/tProjectile23.h"




ATGun23::ATGun23()
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun23.tgun23'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun23.MI_TGun23'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile23.BP_tProjectile23_C"));

	/*Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT20.NI_ShootT20'")));
	Niagara->SetAutoActivate(false);*/

	EnemyNames.Empty();
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");
}




void ATGun23::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG23_EnergyPrice);
		ExtRadius = GInstance->TG23_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG23_Radius);
		Gun_Delay = GInstance->TG23_GunDelay;
		SpawnDelay = GInstance->TG23_SpawnDelay;
		ShootEnergyPrice = GInstance->TG23_ShootEnergyPrice;
	}
	GInstance = nullptr;
}





bool ATGun23::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 30.0) || Dist <= ExtRadius);
}





void ATGun23::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}






void ATGun23::Tracking()
{
}






void ATGun23::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Niagara->GetComponentRotation());
		if (IsValid(Prj))
		{
			Prj->Aim = AimComponent->GetComponentLocation();
			AtProjectile23* Projectile = Cast<AtProjectile23>(Prj);
			if (IsValid(Projectile))
			{
				Projectile->Start();
			}
			Projectile = nullptr;
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}