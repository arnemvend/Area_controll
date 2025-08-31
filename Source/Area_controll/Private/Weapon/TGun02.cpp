// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun02.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



ATGun02::ATGun02()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun02.tgun02'")));
	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun02.MI_TGun02'"));
	GunMesh->SetMaterial(0, GunMaterial);

	GunRMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunRMesh"));
	GunRMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun02gun.tgun02gun'")));
	GunRMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun02gun.MI_TGun02gun'"));
	GunRMesh->SetMaterial(0, GunRMaterial);
	GunRMesh->SetupAttachment(GunMesh);


	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile_02.BP_tProjectile_02_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot0.NI_Shoot0'")));
	Niagara->SetAutoActivate(false);
	Niagara->SetupAttachment(GunRMesh);


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
}




void ATGun02::Rotate(float Amount)
{
	if (IsValid(AimComponent))
	{
		Rot = UKismetMathLibrary::FindLookAtRotation
		(GunRMesh->GetComponentLocation(), AimComponent->GetComponentLocation());
	}
	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
	GunRMesh->SetRelativeRotation(FRotator(Rot.Pitch, 0.0f, 0.0f));
}





void ATGun02::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GunRMesh->GetComponentLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
	GunRMesh->SetRelativeRotation(FRotator(Rot.Pitch, 0.0f, 0.0f));
}





void ATGun02::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		FRotator Rotate;
		Rotate.Roll = GunRMesh->GetComponentRotation().Roll + FMath::FRandRange(Accurary * (-1.0f), Accurary);
		Rotate.Yaw = GunRMesh->GetComponentRotation().Yaw + FMath::FRandRange(Accurary * (-1.0f), Accurary);
		Rotate.Pitch = GunRMesh->GetComponentRotation().Pitch + FMath::FRandRange(Accurary * (-1.0f), Accurary);
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), Rotate);
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}




void ATGun02::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG00_EnergyPrice);
		Accurary = GInstance->TG02_Accurary;
		GunRadius->SetCapsuleRadius(GInstance->TG02_Radius);
		Gun_Delay = GInstance->TG02_GunDelay;
		SpawnDelay = GInstance->TG02_SpawnDelay;
		ShootEnergyPrice = GInstance->TG02_ShootEnergyPrice;
	}
	GInstance = nullptr;
}