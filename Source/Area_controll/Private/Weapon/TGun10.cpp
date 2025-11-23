// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun10.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
//#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



ATGun10::ATGun10()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun10.tgun10'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun10.MI_TGun10'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile10.BP_tProjectile10_C"));

	/*Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT10.NI_ShootT10'")));
	Niagara->SetAutoActivate(false);*/


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Accurary = 0.0f;
}





void ATGun10::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}




void ATGun10::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}




bool ATGun10::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 30.0) || Dist <= ExtRadius);
}




void ATGun10::Fire()
{
	
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		FRotator Rotate = UKismetMathLibrary::FindLookAtRotation
		(Niagara->GetComponentLocation(), AimComponent->GetComponentLocation());
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Rotate);
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}





void ATGun10::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG10_EnergyPrice);
		ExtRadius = GInstance->TG10_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG10_Radius);
		Gun_Delay = GInstance->TG10_GunDelay;
		SpawnDelay = GInstance->TG10_SpawnDelay;
		ShootEnergyPrice = GInstance->TG10_ShootEnergyPrice;
	}
	GInstance = nullptr;
}


