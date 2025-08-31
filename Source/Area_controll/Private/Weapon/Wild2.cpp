// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Wild2.h"
#include "Boom.h"
#include "Core/AreaControll_GameInstance.h"




AWild2::AWild2()
{
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild2.Wild2'")));
	MeshMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_Wild2.MI_Wild2'"));
	Mesh->SetMaterial(0, MeshMaterial);
}



void AWild2::BeginPlay()
{
	Super::BeginPlay();

	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Speed = GInstance->W2_Speed;
		High = GInstance->W2_High;
		MaxHealth = GInstance->W2_MaxHealth;
		Health = MaxHealth;
	}
	GInstance = nullptr;
	CanDamage = true;
}


void AWild2::Destroyed()
{
	if (IsValid(BoomActor) && IsValid(Mesh))
	{
		BoomActor->CreateBoomFunc
		(Mesh->GetComponentLocation(), FRotator::ZeroRotator, BoomActor->LightBoomSystem[2], Color);
	}

	Super::Destroyed();
}