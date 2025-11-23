// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Projectile_0.h"
#include "Boom.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "NiagaraComponent.h"
//#include "NiagaraSystem.h"
#include "Core/AreaControll_GameInstance.h"


AProjectile_0::AProjectile_0()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 1.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(4.0f);
	

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/pr0.pr0'"));
	PMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PMesh->SetupAttachment(Sphere);
	PMesh->SetStaticMesh(MyStaticMesh);
	PMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Weapon/Materials/M_pr0.M_pr0'"));
	PMesh->SetMaterial(0, PMaterial);
	PMesh->SetWorldScale3D(FVector(2.0f, 0.2f, 0.2f));

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_0::OnOverlapBegin);

	InitSpeed = 60.0f;
}



// Called when the game starts or when spawned
void AProjectile_0::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));

	SetterFunc();

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
	if (!IsValid(OtherActor) || !IsValid(OtherComp) || OtherComp->ComponentTags.Num() == 0)
	{
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[1], FColor::White);
		}
		Destroy();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				//spown Boom and destroy
				if (IsValid(BoomActor))
				{
					BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[2], FColor::White);
				}
				Destroy();
				return;
			}
		}
	}
}





void AProjectile_0::SetterFunc()
{
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->G0_Damage;
	}
}




// Called every frame
void AProjectile_0::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}






void AProjectile_0::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(Timer);

	Super::EndPlay(EndPlayReason);
}


