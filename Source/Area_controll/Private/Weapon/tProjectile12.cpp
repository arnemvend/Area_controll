// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile12.h"

#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile12::AtProjectile12()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 10.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(4.0f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_tPr12.NI_tPr12'")));
	Niagara->SetupAttachment(Sphere);
	Niagara->SetAutoActivate(true);

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Speed = 0.4f;
	CustomDTime = 0.04f;
	StartTime = 0.0f;
	Boost = 3.0f;
	CurrentTime = 0.0f;
	DeathDelay = 2.0f;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile12::OnOverlapBegin);
}





void AtProjectile12::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG12_Damage;
	}
	GInstance = nullptr;

	StartOffset = Sphere->GetForwardVector() * Speed;

	//timer for move
	GetWorldTimerManager().SetTimer(Timer, [this]()
		{
			if (IsValid(Component) && IsValid(Sphere))
			{
				//v = v0 + a*t
				Sphere->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation
				(Sphere->GetComponentLocation(), Component->GetComponentLocation()));
				const FVector CurrentOffset = StartOffset + Boost * Sphere->GetForwardVector() * CurrentTime;

				Sphere->AddWorldOffset(CurrentOffset, true);

				CurrentTime += CustomDTime;
			}
			else
			{
				if (IsValid(BoomActor))
				{
					BoomActor->CreateBoomFunc(Sphere->GetComponentLocation(), FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[2], FColor::White);
				}
				Niagara->Deactivate();
				if (IsValid(Sphere))
				{
					Sphere->DestroyComponent();
				}
				DeathFunc();
				if (Timer.IsValid() && GetWorldTimerManager().TimerExists(Timer))
				{
					GetWorldTimerManager().ClearTimer(Timer);
				}
			}
		}, CustomDTime, true);
}






void AtProjectile12::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().ClearTimer(DestroyTimer);

	Super::Destroyed();
}





void AtProjectile12::DeathFunc()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
		{
			Destroy();
		}, DeathDelay, false);
}






void AtProjectile12::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		if (Timer.IsValid() && GetWorldTimerManager().TimerExists(Timer))
		{
			GetWorldTimerManager().ClearTimer(Timer);
		}
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[1], FColor::White);
		}
		Niagara->Deactivate();
		if (IsValid(Sphere))
		{
			Sphere->DestroyComponent();
		}
		DeathFunc();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		if (Timer.IsValid() && GetWorldTimerManager().TimerExists(Timer))
		{
			GetWorldTimerManager().ClearTimer(Timer);
		}
		Niagara->Deactivate();
		if (IsValid(Sphere))
		{
			Sphere->DestroyComponent();
		}
		DeathFunc();
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
				if (Timer.IsValid() && GetWorldTimerManager().TimerExists(Timer))
				{
					GetWorldTimerManager().ClearTimer(Timer);
				}
				if (IsValid(BoomActor))
				{
					BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[2], FColor::White);
				}
				Niagara->Deactivate();
				if (IsValid(Sphere))
				{
					Sphere->DestroyComponent();
				}
				DeathFunc();
				return;
			}
		}
	}
}
