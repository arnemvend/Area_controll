// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/Construction.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
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
	Niagara->SetAutoDestroy(true);
	Niagara->OnSystemFinished.AddDynamic(this, &AConstruction::OnNiagaraSystemFinished);


	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Builder.NI_Builder'"));
	NiagaraBr = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraBuilder"));
	NiagaraBr->SetupAttachment(RootComponent);
	NiagaraBr->SetAsset(NiagaraSystem);
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
	NiagaraBr->SetVariableStaticMesh(TEXT("SousceMesh"), MyStaticMesh);


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
	Health = FMath::Min(Health + Step, Max); //real health
	Health_P = Health_P + Step; //value in the counter
	if (Health_P >= Max)
	{
		CapsuleInternal->DestroyComponent();
		CapsuleExternal->DestroyComponent();
		Tower = GetWorld()->SpawnActor<ATower>(Spowned, GetActorLocation(), FRotator::ZeroRotator);
		if (IsValid(Tower))
		{
			//set health of the spowned tower
			Tower->MaxHealth = GInstance->Tr_MaxHealth;
			Tower->Health = Tower->MaxHealth * (Health / Max);
			Tower->SetDamageFunc();
			Tower->Start(IsYour);
			
			NiagaraBr->DeactivateImmediate();
			GetWorldTimerManager().ClearTimer(Timer0);
			HappyEnd();
		}
	}
}


//start niagara FX and destroy actor
void AConstruction::HappyEnd()
{
	TowerMesh->SetVisibility(false);
	Niagara->Activate();
}



void AConstruction::OnNiagaraSystemFinished(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}





// Called when the game starts or when spawned
void AConstruction::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());

	if (IsValid(GInstance))
	{
		Health = GInstance->Cn_StartHealth; //initial health
		Health_P = Health; //initial counter
		Max = GInstance->Cn_MaxHealth; // max health
		Step = GInstance->Cn_AddHealth; // add value health and counter per time

		OnTakeAnyDamage.AddDynamic(this, &AConstruction::OnTakeDamage);
	}

	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		AddHealthFunc();
	}, 1.0f, true);
}





void AConstruction::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);

	Super::Destroyed();
}

