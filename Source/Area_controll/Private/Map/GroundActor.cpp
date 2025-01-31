// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Map/GroundActor.h"
#include "Kismet/GameplayStatics.h"
#include "Tower/Tower.h"
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





//finds the TowerMenu and disables it
void AGroundActor::Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	TArray<AActor*> Towers;
	ATower* Tow{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Your"), Towers);
	if (Towers.Num() > 0)
	{
		for (int i = 0; i < Towers.Num(); i++)
		{
			if (Towers[i])
			{
				Tow = Cast<ATower>(Towers[i]);
			}
			if (Tow && Tow->IsClicked)
			{
				Tow->DeTouch();
			}
		}
	}
	Towers.Empty();
	Tow = nullptr;
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

