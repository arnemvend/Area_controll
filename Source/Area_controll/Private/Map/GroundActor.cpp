// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Map/GroundActor.h"

// Sets default values
AGroundActor::AGroundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GroundStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Map/Ground.Ground'")); 
	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	GroundMesh->SetupAttachment(RootComponent);
	GroundMesh->SetStaticMesh(GroundStaticMesh);
	GroundMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Map/MI_Grid.MI_Grid'"));
	GroundMesh->SetMaterial(0, GroundMaterial);
}

// Called when the game starts or when spawned
void AGroundActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGroundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

