// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun00.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



ATGun00::ATGun00()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun00.tgun00'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun00.MI_TGun00'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile00.BP_tProjectile00_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT00.NI_ShootT00'")));
	Niagara->SetAutoActivate(false);
	


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
}




bool ATGun00::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 14.0) || Dist <= ExtRadius);
}



void ATGun00::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}




void ATGun00::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
	Rot = UKismetMathLibrary::FindLookAtRotation
	(Niagara->GetComponentLocation(), AimComponent->GetComponentLocation());
	Niagara->SetRelativeRotation(FRotator(Rot.Pitch, 0.0f, 0.0f));
}




void ATGun00::Fire()
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





void ATGun00::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG00_EnergyPrice);
		ExtRadius = GInstance->TG00_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG00_Radius);
		Gun_Delay = GInstance->TG00_GunDelay;
		SpawnDelay = GInstance->TG00_SpawnDelay;
		ShootEnergyPrice = GInstance->TG00_ShootEnergyPrice;
	}
	GInstance = nullptr;
}
