// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/Construction.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Tower/Tower.h"


// Sets default values
AConstruction::AConstruction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/EnergyTower.EnergyTower'"));
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(RootComponent);
	TowerMesh->SetStaticMesh(MyStaticMesh);
	TowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Construct.MI_Construct'"));
	TowerMesh->SetMaterial(0, TowMaterial);


	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Builder.Builder'"));
	RotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotMesh"));
	RotMesh->SetupAttachment(RootComponent);
	RotMesh->SetStaticMesh(MyStaticMesh);
	RotMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Builder.MI_Builder'"));
	RotMesh->SetMaterial(0, RotMaterial);


	CapsuleInternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CapsuleInternal->SetupAttachment(RootComponent);
	CapsuleInternal->SetCapsuleRadius(28.0f);
	CapsuleInternal->SetCapsuleHalfHeight(70.0f);


	Health = 10.0f;
	Max = 30.0f;
	Health_P = 10.0f;


	OnTakeAnyDamage.AddDynamic(this, &AConstruction::OnTakeDamage);
}



//Logic of damage
void AConstruction::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	Health = Health - Damage;
	if (Health <= 0)
	{
		
		Destroy();
	}
}



//Logic of color (Your or Enemy)
void AConstruction::ColorFunc()
{
	DTowMaterial = TowerMesh->CreateDynamicMaterialInstance(0, TowMaterial);
	DTowMaterial->SetVectorParameterValue(TEXT("Color"), Color);
	DRotMaterial = RotMesh->CreateDynamicMaterialInstance(0, RotMaterial);
	DRotMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}




//Logic of building
void AConstruction::AddHealthFunc()
{
	Health = Health + 2.0f; //real health
	Health_P = Health_P + 2.0f; //secondary health
	if (Health_P >= Max)
	{
		const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		Tower = GetWorld()->SpawnActor<ATower>(Spowned, GetActorLocation(), SpawnRotation);
		if (Tower)
		{
			Tower->Health = Tower->MaxHealth * (Health / Max);
			Tower->SetDamageFunc();

			
			Destroy();
		}
	}
}


// Called when the game starts or when spawned
void AConstruction::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		RotMesh->AddWorldRotation(FRotator(0.0f, 0.5f, 0.0));
	}, 0.04f, true);

	GetWorldTimerManager().SetTimer(Timer1, [this]()
	{
		AddHealthFunc();
	}, 1.0f, true);

}




// Called every frame
void AConstruction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AConstruction::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);
}

