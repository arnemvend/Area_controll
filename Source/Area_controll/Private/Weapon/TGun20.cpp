// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun20.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
//#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
//#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"


ATGun20::ATGun20()
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun20.tgun20'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Buildings/Materials/MI_TGun20.MI_TGun20'"));

	GunMesh->SetMaterial(0, GunMaterial);

	GunRadius->ComponentTags.Add("Radius");

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_tProjectile20.BP_tProjectile20_C"));

	/*Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT20.NI_ShootT20'")));
	Niagara->SetAutoActivate(false);*/



	EnemyNames.Empty();
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");

}



/*bool ATGun20::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 30.0) || Dist <= ExtRadius);
}*/



/*void ATGun20::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}*/




/*void ATGun20::Tracking()
{
}*/




void ATGun20::Fire()
{
	if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
	{
		Prj = GetWorld()->SpawnActor<AProjectile>
			(SpownProjectile, Niagara->GetComponentLocation(), FRotator::ZeroRotator);
		if (IsValid(Prj) && IsValid(AimComponent))
		{
			Prj->Aim = FVector(AimComponent->GetComponentLocation().X,
				AimComponent->GetComponentLocation().Y,Accurary);
		}
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
	}
}





void ATGun20::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG20_EnergyPrice);
		ExtRadius = GInstance->TG20_ExtRadius;
		GunRadius->SetCapsuleRadius(GInstance->TG20_Radius);
		Gun_Delay = GInstance->TG20_GunDelay;
		SpawnDelay = GInstance->TG20_SpawnDelay;
		Accurary = GInstance->TG20_Accurary;
		ShootEnergyPrice = GInstance->TG20_ShootEnergyPrice;
	}
	GInstance = nullptr;
}





