// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Projectile_0.h"
#include "Boom.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


AProjectile_0::AProjectile_0()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 2.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(2.0f);

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/pr0.pr0'"));
	PMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PMesh->SetupAttachment(Sphere);
	PMesh->SetStaticMesh(MyStaticMesh);
	PMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/M_pr0.M_pr0'"));
	PMesh->SetMaterial(0, PMaterial);
	PMesh->SetWorldScale3D(FVector(2.0f, 0.2f, 0.2f));

	InitSpeed = 60.0f;
	Damage = 2.0f;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_0::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AProjectile_0::BeginPlay()
{
	Super::BeginPlay();
	FVector FVec = Sphere->GetForwardVector() * InitSpeed;
	//timer for move
	GetWorldTimerManager().SetTimer(Timer, [this, FVec]()
	{
		Sphere->AddWorldOffset(FVec, true);
	}, 0.05f, true);
	
}


//collision logic for different objects
void AProjectile_0::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherComp->ComponentHasTag(TEXT("Internal")) || OtherComp->ComponentHasTag(TEXT("InternalEnemy"))) 
		&& IsValid(OtherActor))
	{
		//logic of damage
		AController* EventInstigator = GetInstigatorController();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, EventInstigator, this, UDamageType::StaticClass());
		//spown Boom and destroy
		EventInstigator = nullptr;
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		ABoom* Boom = GetWorld()->SpawnActor<ABoom>(Spowned, SweepResult.Location, SpawnRotation);
		if (Boom)
		{
			Boom->NiagaraBoomSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_obj.NI_Boom0_obj'"));
	        Boom->NiagaraBoom->SetAsset(Boom->NiagaraBoomSystem);
		    Boom->Boom();
		}
		Boom = nullptr;
		Destroy();
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		ABoom* Boom = GetWorld()->SpawnActor<ABoom>(Spowned, SweepResult.Location, SpawnRotation);
		if (Boom)
		{
			Boom->NiagaraBoomSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_gr.NI_Boom0_gr'"));
	        Boom->NiagaraBoom->SetAsset(Boom->NiagaraBoomSystem);
		    Boom->Boom();
		}
		Boom = nullptr;
		Destroy();
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		ABoom* Boom = GetWorld()->SpawnActor<ABoom>(Spowned, SweepResult.Location, SpawnRotation);
		if (Boom)
		{
			Boom->NiagaraBoomSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_sh.NI_Boom0_sh'"));
	        Boom->NiagaraBoom->SetAsset(Boom->NiagaraBoomSystem);
		    Boom->Boom();
		}
		Boom = nullptr;
		Destroy();
	}
}



// Called every frame
void AProjectile_0::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile_0::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer);
}


