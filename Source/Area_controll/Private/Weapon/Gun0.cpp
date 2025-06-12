// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Gun0.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



AGun0::AGun0()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun0.WildGun0'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_wild0gun.MI_wild0gun'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
	    (nullptr, TEXT("/Game/Weapon/BP_Projectile_0.BP_Projectile_0_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot0.NI_Shoot0'")));

}


//logic of rotate
void AGun0::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), Rot, Amount, true));
}



void AGun0::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance))
	{
		Accurary = GInstance->G0_Accurary;
		Gun_Delay = GInstance->G0_GunDelay;
		GunRadius->SetCapsuleRadius(GInstance->G0_Radius);
	}
	GInstance = nullptr;
}


