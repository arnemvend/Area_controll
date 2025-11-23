// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Wild0.h"
#include "Boom.h"
#include "Core/AreaControll_GameInstance.h"


AWild0::AWild0()
{
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild0.Wild0'")));
	MeshMaterial = LoadObject<UMaterialInterface>
	    (nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_Wild0.MI_Wild0'"));
	Mesh->SetMaterial(0, MeshMaterial);
}





void AWild0::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(BoomActor) && IsValid(Mesh))
	{
		BoomActor->CreateBoomFunc
		(Mesh->GetComponentLocation(), FRotator::ZeroRotator, BoomActor->LightBoomSystem[0], Color);
	}

	Super::EndPlay(EndPlayReason);
}





void AWild0::BeginPlay()
{
	Super::BeginPlay();

	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Speed = GInstance->W0_Speed;
		High = GInstance->W0_High;
		MaxHealth = GInstance->W0_MaxHealth;
		Health = MaxHealth;
	}
	GInstance = nullptr;
	CanDamage = true;
}
