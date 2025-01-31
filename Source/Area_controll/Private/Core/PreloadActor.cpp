// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/PreloadActor.h"
#include "NiagaraSystem.h"


// Sets default values
APreloadActor::APreloadActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	RMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Repeater.Repeater'"));
	RNSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Repeater.NI_Repeater'"));


	//UNIT 0 ----------------------------------------------------->
	Health.Add(10.0f);
	Speed.Add(25.0f);
	High.Add(100.0f);
	SM_Main.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild0.Wild0'")));
	MI_Main.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Wild0.MI_Wild0'")));
	//Unit 0 Gun
	Gun_SM.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun0.WildGun0'")));
	Gun_MI.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild0gun.MI_wild0gun'")));
	Gun_NS.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot0.NI_Shoot0'")));
	Gun_Radius.Add(300.0f);
	Gun_Accurary.Add(12.0f);
	Gun_Speed.Add(0.25f);
	Gun_BP.Add(LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/BP_Projectile_0.BP_Projectile_0_C")));


	//UNIT 1 ----------------------------------------------------->
	Health.Add(20.0f);
	Speed.Add(20.0f);
	High.Add(200.0f);
	SM_Main.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/Wild1.Wild1'")));
	MI_Main.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Wild1.MI_Wild1'")));
	//Unit 1 Gun
	Gun_SM.Add(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildGun1.WildGun1'")));
	Gun_MI.Add(LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_wild1gun.MI_wild1gun'")));
	Gun_NS.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Shoot1.NI_Shoot1'")));
	Gun_Radius.Add(350.0f);
	Gun_Accurary.Add(0.0f);
	Gun_Speed.Add(3.0f);
	Gun_BP.Add(LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/BP_Projectile_1.BP_Projectile_1_C")));
	
}




// Called when the game starts or when spawned
void APreloadActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APreloadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



