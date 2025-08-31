// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Gun2.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Weapon/Projectile.h"




AGun2::AGun2()
{
	SpownProjectile = LoadClass<AProjectile>
		(nullptr, TEXT("/Game/Weapon/BP_Projectile_2.BP_Projectile_2_C"));

	SpawnNiagara->DestroyComponent();

	Accurary = 0.0f;
	BaseRotatePlayRate = 2.0f;
}




void AGun2::TimerElapsed()
{
}

void AGun2::Rotate(float Amount)
{
}

void AGun2::FreeRotate(float Amount)
{
}

void AGun2::Tracking()
{
}





void AGun2::Fire()
{
	Prj = GetWorld()->SpawnActor<AProjectile>
		(SpownProjectile, GunMesh->GetComponentLocation(), 
			FRotator(0.0f, GunMesh->GetComponentRotation().Yaw, 0.0f));
	if (IsValid(Prj) && IsValid(AimComponent))
	{
		Prj->Component = AimComponent;
	}
}





void AGun2::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance))
	{
		Gun_Delay = GInstance->G1_GunDelay;
		GunRadius->SetCapsuleRadius(GInstance->G1_Radius);
	}
	GInstance = nullptr;
}
