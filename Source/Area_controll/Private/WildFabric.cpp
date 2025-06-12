// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "WildFabric.h"

#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Wild.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
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



	CaplsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CaplsuleComponent"));
	CaplsuleComponent->SetupAttachment(RootComponent);
	CaplsuleComponent->ComponentTags.Add(TEXT("InternalWild"));
	


	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetRelativeLocation(FVector(0.0f, 0.0f, 72.0f));
	Niagara->SetRelativeRotation(FRotator(-54.0f, 0.0f, 0.0f));
	NiagaraNetSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_SpownTech.NI_SpownTech'"));
	Niagara->SetAsset(NiagaraNetSystem);

	Type = 0;
	CanDamage = false;
	Wild = nullptr;


	OnTakeAnyDamage.AddDynamic(this, &AWildFabric::OnTakeDamage);
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
	Wild =  GetWorld()->SpawnActor<AWild>(Spowned[Type], Loc, GetActorRotation());
	if (IsValid(Wild))
	{
		Wild->AimCoord = AimCoord;
		Wild->Color = GInstance->WildColor;
		Wild->Start();
		Wild = nullptr;
	}
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			Niagara->DeactivateImmediate();
		}, 3.0f, false);
}






void AWildFabric::SetDamageFunc()
{
	if (Health <= 0)
	{
		Destroy();
		return;
	}
	float const Alpha = FMath::Max(0.1f, Health / MaxHealth);
	if (DMaterial)
	{
		DMaterial->SetScalarParameterValue(TEXT("Damage"), Alpha);
	}
}




void AWildFabric::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health > 0 && CanDamage)
	{
		Health = Health - Damage;
		SetDamageFunc();
	}
}





void AWildFabric::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	DMaterial = FabricMesh->CreateDynamicMaterialInstance(0, Material);
	DMaterial->SetVectorParameterValue(TEXT("LightParam"), GInstance->WildColor);

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));

	if (IsValid(GInstance))
	{
		MaxHealth = GInstance->WF_MaxHealth;
		Health = MaxHealth;
		CanDamage = true;
	}
}




void AWildFabric::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer0);
	if (IsValid(BoomActor) && IsValid(GInstance))
	{
		BoomActor->CreateBoomFunc(GetActorLocation(), GetActorRotation(),
			BoomActor->BuildBoomSystem[2], GInstance->WildColor);
	}

	CaplsuleComponent->DestroyComponent();

	Super::Destroyed();
}




void AWildFabric::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

