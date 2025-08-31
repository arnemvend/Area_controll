// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Gun1.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"



AGun1::AGun1()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun1.WildGun1'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_wild1gun.MI_wild1gun'"));

	GunMesh->SetMaterial(0, GunMaterial);

	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_Projectile_1.BP_Projectile_1_C"));

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot1.NI_Shoot1'")));

	SpawnNiagara->DestroyComponent();

	Accurary = 0.0f;
}


//logic of rotate
void AGun1::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), Rot, Amount, true));
}




void AGun1::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance))
	{
		Gun_Delay = GInstance->G1_GunDelay;
		GunRadius->SetCapsuleRadius(GInstance->G1_Radius);
	}
	GInstance = nullptr;
}





