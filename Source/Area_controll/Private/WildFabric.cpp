// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "WildFabric.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Wild.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWildFabric::AWildFabric()
{
 
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildFabric.WildFabric'"));
	FabricMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FabricMesh"));
	FabricMesh->SetupAttachment(RootComponent);
	FabricMesh->SetStaticMesh(MyStaticMesh);
	FabricMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/M_WildFabric.M_WildFabric'"));
	FabricMesh->SetMaterial(0, Material);

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'"));
	SquareMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareMesh"));
	SquareMesh->SetupAttachment(RootComponent);
	SquareMesh->SetStaticMesh(MyStaticMesh);
	SquareMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.5f));
	SquareMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	SquareMesh->SetRelativeScale3D(FVector(3.45f, 3.45f, 1.0f));
	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Shadow_fabric.MI_Shadow_fabric'"));
	SquareMesh->SetMaterial(0, Material);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetRelativeLocation(FVector(0.0f, 0.0f, 72.0f));
	Niagara->SetRelativeRotation(FRotator(-54.0f, 0.0f, 0.0f));
	NiagaraNetSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_SpownTech.NI_SpownTech'"));
	Niagara->SetAsset(NiagaraNetSystem);

	Type = 0;
}


void AWildFabric::RotFunc()
{
	const FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AimCoord);
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}

void AWildFabric::CreateFunc()
{
	Niagara->Activate();
	const FVector Loc = GetActorLocation() + FVector(0.0f, 0.0f, 72.0f);
	AWild* Wild =  GetWorld()->SpawnActor<AWild>(Spowned, Loc, GetActorRotation());
	Wild->AimCoord = AimCoord;
	Wild->Type = Type;
	Wild->Start();
	Wild = nullptr;
}


// Called when the game starts or when spawned
void AWildFabric::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWildFabric::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

