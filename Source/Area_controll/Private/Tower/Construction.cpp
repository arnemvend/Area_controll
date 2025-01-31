// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/Construction.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
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


	CapsuleInternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InternalCollisionComponent"));
	CapsuleInternal->SetupAttachment(RootComponent);
	CapsuleInternal->SetCapsuleRadius(28.0f);
	CapsuleInternal->SetCapsuleHalfHeight(70.0f);
	CapsuleInternal->ComponentTags.Add(TEXT("Internal"));


	CapsuleExternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ExternalCollisionComponent"));
	CapsuleExternal->SetupAttachment(RootComponent);
	CapsuleExternal->SetCapsuleRadius(200.0f);
	CapsuleExternal->SetCapsuleHalfHeight(300.0f);


	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Construction.NI_Construction'"));
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAsset(NiagaraSystem);
	Niagara->SetAutoActivate(false);


	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Builder.NI_Builder'"));
	NiagaraBr = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraBuilder"));
	NiagaraBr->SetupAttachment(RootComponent);
	NiagaraBr->SetAsset(NiagaraSystem);


	Health = 10.0f; //initial health
	Health_P = Health; //initial counter
	Max = 30.0f; // max health
	Step = 2.0f; // add value health and counter per time

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
	DTowMaterial->SetVectorParameterValue(TEXT("CreatorColor"), Color);
	Niagara->SetVariableLinearColor(TEXT("Color"), Color);
	NiagaraBr->SetVariableLinearColor(TEXT("Color"), Color);


	//it can't run in BeginPlay in this time. It will fix in future
	if (IsYour)
	{
		CapsuleExternal->ComponentTags.Add(TEXT("External"));
	}
	else
	{
		CapsuleExternal->ComponentTags.Add(TEXT("ExternalEnemy"));
	}
}




//Logic of building
void AConstruction::AddHealthFunc()
{
	Health = Health + Step; //real health
	Health_P = Health_P + Step; //value in the counter
	if (Health_P >= Max)
	{
		const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		Tower = GetWorld()->SpawnActor<ATower>(Spowned, GetActorLocation(), SpawnRotation);
		if (Tower)
		{
			Tower->Start(IsYour);
			//set health of the spowned tower
			Tower->Health = Tower->MaxHealth * (Health / Max);
			Tower->SetDamageFunc();
			
			NiagaraBr->DeactivateImmediate();
			GetWorldTimerManager().ClearTimer(Timer1);
			HappyEnd();
		}
	}
}


//start niagara FX and destroy actor
void AConstruction::HappyEnd()
{
	Niagara->Activate();
	TowerMesh->DestroyComponent();
	CapsuleInternal->DestroyComponent();
	CapsuleExternal->DestroyComponent();
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			Niagara->DeactivateImmediate();
			Destroy();
		}, 2.0f, false);
}


// Called when the game starts or when spawned
void AConstruction::BeginPlay()
{
	Super::BeginPlay();

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

