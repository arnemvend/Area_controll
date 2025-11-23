// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Wild1.h"
#include "Boom.h"
#include "Core/AreaControll_GameInstance.h"


AWild1::AWild1()
{
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild1.Wild1'")));
	MeshMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_Wild1.MI_Wild1'"));
	Mesh->SetMaterial(0, MeshMaterial);
}




void AWild1::BeginPlay()
{
	Super::BeginPlay();

	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Speed = GInstance->W1_Speed;
		High = GInstance->W1_High;
		MaxHealth = GInstance->W1_MaxHealth;
		Health = MaxHealth;
	}
	GInstance = nullptr;
	CanDamage = true;
}





void AWild1::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(BoomActor) && IsValid(Mesh))
	{
		BoomActor->CreateBoomFunc
		(Mesh->GetComponentLocation(), FRotator::ZeroRotator, BoomActor->LightBoomSystem[1], Color);
	}

	Super::EndPlay(EndPlayReason);
}


