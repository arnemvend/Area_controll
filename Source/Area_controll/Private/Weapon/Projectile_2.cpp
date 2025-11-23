// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Projectile_2.h"
#include "Boom.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"




AProjectile_2::AProjectile_2()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 10.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshShadow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	MeshShadow->SetupAttachment(RootComponent);
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'"));
	MeshShadow->SetStaticMesh(MyStaticMesh);
	MeshShadow->SetMaterial(0, LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_Shadow_Light.MI_Shadow_Light'")));

	MeshShadow->SetVisibility(false);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->ComponentTags.Add("WildLighter");

	NSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Proj2.NI_Proj2'"));

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(Trigger);
	Niagara->SetAsset(NSystem);
	Niagara->SetAutoActivate(true);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_2::OnOverlapBegin);
	OnTakeAnyDamage.AddDynamic(this, &AProjectile_2::OnTakeDamage);

	CurrentStep = 0;
	CustomHigh = 15.0f;
	CustomDeltaTime = 0.0333f;
	RotateMulti = 2.0f;
	TimeOfFall = 1.5f;
	CanLight = false;
}



void AProjectile_2::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->G2_Damage;
		Speed = GInstance->G2_PrSpeed;
		MaxHealth = GInstance->G2_PrHealth;
		Health = MaxHealth;
	}
	GInstance = nullptr;

	CurrentRot = FRotator(0.0f, Trigger->GetComponentRotation().Yaw, 0.0f);
	CurrentHigh = Trigger->GetComponentLocation().Z;

	FinishStep = FMath::FloorToInt(TimeOfFall / CustomDeltaTime);
	HighStep = (CurrentHigh - CustomHigh) / FinishStep;

	GetWorldTimerManager().SetTimer(CanLightTimer, [this]()
		{
			if (IsValid(Component))
			{
				CanLight = true;
				AimRot = FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(Trigger->GetComponentLocation(), 
					Component->GetComponentLocation()).Yaw, 0.0f);
				GetWorldTimerManager().ClearTimer(CanLightTimer);
			}
		}, CustomDeltaTime, true);

	GetWorldTimerManager().SetTimer(FallTimer, [this]()
		{
		    if (CurrentStep <= FinishStep)
		    {
				if (CanLight)
				{
					Trigger->AddWorldOffset(FVector(0.0f, 0.0f, -HighStep), true);
					float Value = FMath::Min(1.0f, RotateMulti * CurrentStep / FinishStep);
					Trigger->SetWorldRotation(UKismetMathLibrary::RLerp(CurrentRot, AimRot, Value, true), true);
					CurrentStep++;
				}
		    }
			else
			{
				GetWorldTimerManager().ClearTimer(FallTimer);
			}
		}, CustomDeltaTime, true);

	GetWorldTimerManager().SetTimer(WayTimer, [this]()
		{
			if (CanLight)
			{
				Trigger->AddWorldOffset(Trigger->GetForwardVector() * Speed, true);
				MeshShadow->SetWorldLocation(FVector(Trigger->GetComponentLocation().X, Trigger->GetComponentLocation().Y, 0.1f));
				if (!MeshShadow->IsVisible())
				{
					MeshShadow->SetVisibility(true);
				}
			}
		}, CustomDeltaTime, true, TimeOfFall / RotateMulti);
}




void AProjectile_2::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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
					BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj2BoomSystem, FColor::White);
				}
				Destroy();
				return;
			}
		}
	}
}




void AProjectile_2::OnTakeDamage(AActor* DamagedActor, float Damage0, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health > 0.0f)
	{
		Health = Health - Damage0;
		if (Health <= 0.0f)
		{
			if (IsValid(BoomActor))
			{
				BoomActor->CreateBoomFunc(Trigger->GetComponentLocation(), FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[2], FColor::White);
			}
			Destroy();
			return;
		}
	}
}





void AProjectile_2::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(FallTimer);
	GetWorldTimerManager().ClearTimer(WayTimer);
	GetWorldTimerManager().ClearTimer(CanLightTimer);

	Super::EndPlay(EndPlayReason);
}










