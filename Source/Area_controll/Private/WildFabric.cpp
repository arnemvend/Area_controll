// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "WildFabric.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Wild.h"
#include "Core/PreloadActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWildFabric::AWildFabric()
{
 
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'"));
	SquareMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareMesh"));
	SquareMesh->SetupAttachment(RootComponent);
	SquareMesh->SetStaticMesh(MyStaticMesh);
	SquareMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.5f));
	SquareMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	SquareMesh->SetRelativeScale3D(FVector(3.45f, 3.45f, 1.0f));
	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_Shadow_fabric.MI_Shadow_fabric'"));
	SquareMesh->SetMaterial(0, Material);



	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/WildFabric.WildFabric'"));
	FabricMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FabricMesh"));
	FabricMesh->SetupAttachment(RootComponent);
	FabricMesh->SetStaticMesh(MyStaticMesh);
	FabricMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/MI_WildFabric.MI_WildFabric'"));
	FabricMesh->SetMaterial(0, Material);
	


	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetRelativeLocation(FVector(0.0f, 0.0f, 72.0f));
	Niagara->SetRelativeRotation(FRotator(-54.0f, 0.0f, 0.0f));
	NiagaraNetSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_SpownTech.NI_SpownTech'"));
	Niagara->SetAsset(NiagaraNetSystem);

	Type = 0;
	Wild = nullptr;
}


//set rotate to AimCoord
void AWildFabric::RotFunc()
{
	const FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AimCoord);
	SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
}



//spown Wild and set it's parameters
void AWildFabric::CreateFunc()
{
	Niagara->Activate();//spown FX
	const FVector Loc = GetActorLocation() + FVector(0.0f, 0.0f, 72.0f);
	Wild =  GetWorld()->SpawnActor<AWild>(Spowned, Loc, GetActorRotation());
	if (Wild)
	{
		Wild->AimCoord = AimCoord;
		Wild->Type = Type;
		Wild->Start();
		Wild = nullptr;
	}
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			Niagara->DeactivateImmediate();
		}, 3.0f, false);
}



void AWildFabric::BeginPlay()
{
	Super::BeginPlay();

	APreloadActor* PActor = Cast<APreloadActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APreloadActor::StaticClass()));
	DMaterial = FabricMesh->CreateDynamicMaterialInstance(0, Material);
	DMaterial->SetVectorParameterValue(TEXT("Emissive"), PActor->WildColor);
	PActor = nullptr;
}

void AWildFabric::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
}


void AWildFabric::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

