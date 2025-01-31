// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Projectile_1.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Boom.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile_1::AProjectile_1()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 2.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sphere0 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent_0"));
	Sphere0->SetupAttachment(RootComponent);
	Sphere0->SetSphereRadius(4.0f);

	Sphere1 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent_1"));
	Sphere1->SetupAttachment(RootComponent);
	Sphere1->SetSphereRadius(4.0f);

	Sphere2 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent_2"));
	Sphere2->SetupAttachment(RootComponent);
	Sphere2->SetSphereRadius(4.0f);

	Sphere3 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent_3"));
	Sphere3->SetupAttachment(RootComponent);
	Sphere3->SetSphereRadius(4.0f);

	Sphere4 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent_4"));
	Sphere4->SetupAttachment(RootComponent);
	Sphere4->SetSphereRadius(4.0f);


	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Claster.NI_Claster'"));

	Niagara0 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_0"));
	Niagara0->SetupAttachment(RootComponent);
	Niagara0->SetAsset(NiagaraSystem);
	Niagara0->SetAutoActivate(false);

	Niagara1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_1"));
	Niagara1->SetupAttachment(RootComponent);
	Niagara1->SetAsset(NiagaraSystem);
	Niagara1->SetAutoActivate(false);

	Niagara2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_2"));
	Niagara2->SetupAttachment(RootComponent);
	Niagara2->SetAsset(NiagaraSystem);
	Niagara2->SetAutoActivate(false);

	Niagara3 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_3"));
	Niagara3->SetupAttachment(RootComponent);
	Niagara3->SetAsset(NiagaraSystem);
	Niagara3->SetAutoActivate(false);

	Niagara4 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent_4"));
	Niagara4->SetupAttachment(RootComponent);
	Niagara4->SetAsset(NiagaraSystem);
	Niagara4->SetAutoActivate(false);

	NiagaraArr.Add(Niagara0);
	NiagaraArr.Add(Niagara1);
	NiagaraArr.Add(Niagara2);
	NiagaraArr.Add(Niagara3);
	NiagaraArr.Add(Niagara4);

	NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ClasterBomb.NI_ClasterBomb'"));
	NiagaraClasterBomb = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraClasterBom"));
	NiagaraClasterBomb->SetupAttachment(RootComponent);
	NiagaraClasterBomb->SetAsset(NiagaraSystem);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 150.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;


	Damage = 10.0f;
	Splash = 25.0f;
	CanBoom = false;


	Sphere0->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_1::OnOverlapBegin0);
	Sphere1->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_1::OnOverlapBegin1);
	Sphere2->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_1::OnOverlapBegin2);
	Sphere3->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_1::OnOverlapBegin3);
	Sphere3->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_1::OnOverlapBegin4);

	SphereArr.Add(Sphere0);
	SphereArr.Add(Sphere1);
	SphereArr.Add(Sphere2);
	SphereArr.Add(Sphere3);
	SphereArr.Add(Sphere4);
}






//This function is triggered when a sphere collides with an object
void AProjectile_1::PostReact(USphereComponent* Sphere, UNiagaraComponent* Niagara, FVector Loc)
{
	SphereArr.Remove(Sphere);
	Sphere->DestroyComponent();
	if (NiagaraClasterBomb->IsActive())
	{
		NiagaraClasterBomb->DeactivateImmediate();
	}
	if (CanBoom)
	{
		Niagara->SetVariablePosition(FName(TEXT("EndVector")), Loc);
		Niagara->Activate();
	}
	//if CanBoom = false bomb is stops
	else
	{
		ProjectileMovement->StopMovementImmediately();
	}
}









void AProjectile_1::React(AActor* OtherActor, UPrimitiveComponent* OtherComp, USphereComponent* Sphere,
	UNiagaraComponent* Niagara, FVector Loc)
{
	if ((OtherComp->ComponentHasTag(TEXT("Internal")) || OtherComp->ComponentHasTag(TEXT("InternalEnemy"))) 
		&& IsValid(OtherActor))
	{
		//logic of damage
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		//spown Boom and destroy component
		BoomActor->CreateBoomFunc(Loc, FRotator::ZeroRotator, BoomActor->Proj1BoomSystem[1], FColor::White);
		PostReact(Sphere, Niagara, Loc);
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		BoomActor->CreateBoomFunc(Loc, FRotator::ZeroRotator, BoomActor->Proj1BoomSystem[0], FColor::White);
		PostReact(Sphere, Niagara, Loc);
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		FVector Location;
		if (CanBoom)
		{
			Location = Loc;
		}
		else
		{
			Location = Sphere->GetComponentLocation();
		}
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), Location);
		BoomActor->CreateBoomFunc(Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		PostReact(Sphere, Niagara, Location);
	}
}








//collision logic for every spheres
void AProjectile_1::OnOverlapBegin0(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	React(OtherActor, OtherComp, Sphere0, Niagara0, SweepResult.Location);
}

void AProjectile_1::OnOverlapBegin1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	React(OtherActor, OtherComp, Sphere1, Niagara1, SweepResult.Location);
}

void AProjectile_1::OnOverlapBegin2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	React(OtherActor, OtherComp, Sphere2, Niagara2, SweepResult.Location);
}

void AProjectile_1::OnOverlapBegin3(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	React(OtherActor, OtherComp, Sphere3, Niagara3, SweepResult.Location);
}

void AProjectile_1::OnOverlapBegin4(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	React(OtherActor, OtherComp, Sphere4, Niagara4, SweepResult.Location);
}










// Called when the game starts or when spawned
void AProjectile_1::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));

	FRotator Rot = FRotator::ZeroRotator;
	//set rotation for every spheres
	for (int i = 0; i < SphereArr.Num(); i++)
	{
		Rot.Pitch = UKismetMathLibrary::RandomFloatInRange(-0.65f * Splash, 0.65f * Splash);
		Rot.Yaw = UKismetMathLibrary::RandomFloatInRange(-1.0f * Splash, Splash);
		SphereArr[i]->SetRelativeRotation(Rot);
	}
	//If the bomb has not collided before the end of the delay
	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		ProjectileMovement->StopMovementImmediately();
		CanBoom = true;
		for (int i = 0; i < NiagaraArr.Num(); i++)
		{
			NiagaraArr[i]->SetVariablePosition(FName(TEXT("Start")), GetActorLocation());
		}
		//timer for the movement of spheres
		GetWorldTimerManager().SetTimer(Timer1, [this]()
	    {
		    for (int i = 0; i < SphereArr.Num(); i++)
		    {
				if (SphereArr.Num() > 0)
				{
					if (IsValid(SphereArr[i]))
					{
						SphereArr[i]->AddWorldOffset(SphereArr[i]->GetForwardVector() * 150.0f, true);
					}
				}
				//if all spheres are destroyed, the actor is destroyed
				else
				{
					Destroy();
				}
		    }
	    }, 0.05f, true);
	}, 0.5f, false);
}



// Called every frame
void AProjectile_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile_1::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);
}


