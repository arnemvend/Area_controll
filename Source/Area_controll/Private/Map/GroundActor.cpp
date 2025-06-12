// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Map/GroundActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


// Sets default values
AGroundActor::AGroundActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	GroundStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'")); 
	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	GroundMesh->SetupAttachment(RootComponent);
	GroundMesh->SetStaticMesh(GroundStaticMesh);
	GroundMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Map/MI_Grid.MI_Grid'"));
	GroundMesh->SetMaterial(0, GroundMaterial);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	OnInputTouchBegin.AddDynamic(this, &AGroundActor::Touch);
}


FOnDeTouchDelegate& AGroundActor::GetOnDeTouchDelegate()
{
	return OnDeTouchDelegate;
}

//unusable delegate for this actor
FOnShieldDelegate& AGroundActor::GetOnShieldDelegate()
{
	return OnShieldDelegate;
}


//finds the TowerMenu and disables it
void AGroundActor::Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	OnDeTouchDelegate.Broadcast(0);
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



