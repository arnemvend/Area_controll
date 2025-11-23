// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile23.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tower/Tower.h"


AtProjectile23::AtProjectile23()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 50.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ExtSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExternalCollisionSphere"));
	ExtSphere->SetupAttachment(RootComponent);
	ExtSphere->SetSphereRadius(10.0f);

	IntSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InternalCollisionSphere"));
	IntSphere->SetupAttachment(ExtSphere);
	

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Box->SetupAttachment(RootComponent);
	

	NiagaraStart = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraStart"));
	NiagaraStart->SetupAttachment(ExtSphere);
	NiagaraStart->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_DarkOrb.NI_DarkOrb'")));
	NiagaraStart->SetAutoActivate(false);


	NiagaraCrowl = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraCrowl"));
	NiagaraCrowl->SetupAttachment(Box);
	NiagaraCrowl->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_CrowlT23.NI_CrowlT23'")));
	NiagaraCrowl->SetAutoActivate(false);
	NiagaraCrowl->SetEmitterEnable(TEXT("Circle"), false);
	NiagaraCrowl->OnSystemFinished.AddDynamic(this, &AtProjectile23::OnNiagaraSystemFinished);


	EnemyNames.Empty();
	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");

	Speed = 1.3f;
}




void AtProjectile23::BeginPlay()
{
	Super::BeginPlay();

	IntSphere->SetSphereRadius(ExtSphere->GetScaledSphereRadius() / 2.0f);
	IntSphere->SetRelativeLocation(FVector(IntSphere->GetScaledSphereRadius(), 0.0f, 0.0f));

	Box->SetBoxExtent(FVector(0.0f, ExtSphere->GetScaledSphereRadius(), ExtSphere->GetScaledSphereRadius()));
	Box->SetRelativeLocation(FVector(-ExtSphere->GetScaledSphereRadius(), 0.0f, 0.0f));

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG23_Damage;
		LifeTime = GInstance->TG23_LifeTime;
		DamageRadius = GInstance->TG23_DamageRadius;
		DamageTime = GInstance->TG23_DamageTime;
		ExtRadius = GInstance->TG23_ExtRadius;
	}
	GInstance = nullptr;

	NiagaraStart->SetVariableFloat(TEXT("LifeTime"), 0.5f + 0.033f * ExtRadius / Speed);
	NiagaraStart->SetVariableFloat(TEXT("High"), 0.1f);
	NiagaraStart->Activate();

	GetWorldTimerManager().SetTimer(LifeTimer, [this]()
		{
		    if (NiagaraCrowl->IsActive())
		    {
				NiagaraCrowl->Deactivate();
		    }
			else
			{
				Destroy();
			}
			
		}, LifeTime, false);
}



//First Round. Fly
void AtProjectile23::Start()
{
	StartLenght = (FVector2D(GetActorLocation().X, GetActorLocation().Y) - FVector2D(Aim.X, Aim.Y)).Length();

	IntSphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile23::IntOnOverlapBegin0);
	ExtSphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile23::ExtOnOverlapBegin0);

	GetWorldTimerManager().SetTimer(StartTimer, [this]()
		{
			const float L = (FVector2D(GetActorLocation().X, GetActorLocation().Y) - 
				FVector2D(ExtSphere->GetComponentLocation().X, ExtSphere->GetComponentLocation().Y)).Length();
		    if (L <= ExtRadius)
		    {
				ExtSphere->AddRelativeLocation(FVector(Speed, 0.0f, 0.0f), true);
				Box->AddRelativeLocation(FVector(Speed, 0.0f, 0.0f), true);

				const float Z = GetActorLocation().Z * FMath::Sqrt(1 - (L / ExtRadius));//f(x) = H(1 - x/S)^0.5
				ExtSphere->SetWorldLocation(FVector(ExtSphere->GetComponentLocation().X, ExtSphere->GetComponentLocation().Y, Z), true);
				Box->SetWorldLocation(FVector(Box->GetComponentLocation().X, Box->GetComponentLocation().Y, Z), true);
		    }
			else if (StartTimer.IsValid() && GetWorldTimerManager().IsTimerActive(StartTimer))
			{
				ExtSphere->SetWorldLocation(FVector(ExtSphere->GetComponentLocation().X, ExtSphere->GetComponentLocation().Y, 0.0f), true);
				Box->SetWorldLocation(FVector(Box->GetComponentLocation().X, Box->GetComponentLocation().Y, 0.0f), true);

				IntSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AtProjectile23::IntOnOverlapBegin0);

				CrawlFunc();
				GetWorldTimerManager().ClearTimer(StartTimer);
			}
		}, 0.033f, true);
}




//Second Round. Crawl
void AtProjectile23::CrawlFunc()
{
	IntSphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile23::IntOnOverlapBegin1);
	IntSphere->OnComponentEndOverlap.AddDynamic(this, &AtProjectile23::IntOnOverlapEnd);

	NiagaraStart->Deactivate();
	NiagaraCrowl->Activate();

	GetWorldTimerManager().SetTimer(DamageTimer, [this]()
		{
			ExtSphere->GetOverlappingComponents(OverlappedComponents);
			Box->GetOverlappingComponents(TempComponents);
			OverlappedComponents.Append(TempComponents);
			TempComponents.Empty();

			if (OverlappedComponents.Num() > 0 && EnemyNames.Num() > 0)
			{
				for (int k = 0; k < OverlappedComponents.Num(); k++)
				{
					for (int n = 0; n < EnemyNames.Num(); n++)
					{
						if (OverlappedComponents.IsValidIndex(k) && 
							IsValid(OverlappedComponents[k]) && 
							OverlappedComponents[k]->ComponentHasTag(EnemyNames[n]))
						{
							DamagedComponents.AddUnique(OverlappedComponents[k]);
						}
					}
				}
			}
			OverlappedComponents.Empty();

			if (DamagedComponents.Num() > 0 && IsValid(BoomActor))
			{
				for (int t = 0; t < DamagedComponents.Num(); t++)
				{
					if (DamagedComponents.IsValidIndex(t) && IsValid(DamagedComponents[t]))
					{
						Tower = Cast<ATower>(DamagedComponents[t]->GetOwner());
						if (IsValid(Tower))
						{
							if (Tower->CanDamage)
							{
								UGameplayStatics::ApplyDamage
								(Tower, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
								BoomActor->CreateBoomFunc
								(Tower->GetActorLocation(), FRotator::ZeroRotator, BoomActor->ProjT23BoomSystem[1], FColor::White);
							}
						}
						else
						{
							UGameplayStatics::ApplyDamage
							(DamagedComponents[t]->GetOwner(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());
							BoomActor->CreateBoomFunc
							(DamagedComponents[t]->GetOwner()->GetActorLocation(), FRotator::ZeroRotator, BoomActor->ProjT23BoomSystem[1], FColor::White);
						}
						Tower = nullptr;
					}
				}
			}
			DamagedComponents.Empty();
		}, DamageTime, true);

	GetWorldTimerManager().SetTimer(MoveTimer, [this]()
		{
			const float L = (FVector2D(GetActorLocation().X, GetActorLocation().Y) -
				FVector2D(ExtSphere->GetComponentLocation().X, ExtSphere->GetComponentLocation().Y)).Length();
			if (L <= StartLenght)
			{
				ExtSphere->AddRelativeLocation(FVector(Speed, 0.0f, 0.0f), true);
				Box->AddRelativeLocation(FVector(Speed / 2.0f, 0.0f, 0.0f), true);
				Box->SetBoxExtent(Box->GetScaledBoxExtent() + FVector(Speed / 2.0f, 0.0f, 0.0f));
				NiagaraCrowl->SetVariableFloat(TEXT("Lenght"), 2.0f * Box->GetScaledBoxExtent().X);
			}
			else if (MoveTimer.IsValid() && GetWorldTimerManager().IsTimerActive(MoveTimer))
			{
				ExtSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AtProjectile23::ExtOnOverlapBegin0);
				AreaFunc();
				GetWorldTimerManager().ClearTimer(MoveTimer);
			}
		}, 0.033f, true);
}




//Third Round. Dark Area
void AtProjectile23::AreaFunc()
{
	IntSphere->DestroyComponent();

	Box->SetBoxExtent(Box->GetScaledBoxExtent() + FVector(- (DamageRadius - ExtSphere->GetScaledSphereRadius()) / 2.0f, 0.0f, 0.0f));
	Box->AddRelativeLocation(FVector(- (DamageRadius - ExtSphere->GetScaledSphereRadius()) / 2.0f, 0.0f, 0.0f), true);
	NiagaraCrowl->SetVariableFloat(TEXT("Lenght"), 2.0f * Box->GetScaledBoxExtent().X);
	NiagaraCrowl->SetVariableFloat(TEXT("Radius"), DamageRadius);
	NiagaraCrowl->SetVariableFloat(TEXT("MidPoint"), DamageRadius + Box->GetScaledBoxExtent().X);
	NiagaraCrowl->SetEmitterEnable(TEXT("Circle"), true);

	ExtSphere->SetSphereRadius(DamageRadius);
}





void AtProjectile23::ExtOnOverlapBegin0(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SweepResult.Location, OtherActor->GetActorLocation());
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->ProjT23BoomSystem[0], FColor::White);
		}
	}
}






void AtProjectile23::IntOnOverlapBegin0(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), OverlappedComp->GetComponentLocation());
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
	}
}





void AtProjectile23::IntOnOverlapBegin1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		if (MoveTimer.IsValid() && !GetWorldTimerManager().IsTimerPaused(MoveTimer))
		{
			GetWorldTimerManager().PauseTimer(MoveTimer);
		}
	}
}





void AtProjectile23::IntOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IntSphere->GetOverlappingComponents(ShieldComponents);

	if (ShieldComponents.Num() == 0)
	{
		return;
	}
	for (int i = ShieldComponents.Num() - 1; i >= 0; --i)
	{
		if (ShieldComponents.IsValidIndex(i) && IsValid(ShieldComponents[i]) && ShieldComponents[i]->ComponentHasTag(TEXT("Shield")))
		{
			ShieldComponents.Empty();
			return;
		}
	}
	if (MoveTimer.IsValid() && GetWorldTimerManager().IsTimerPaused(MoveTimer))
	{
		GetWorldTimerManager().UnPauseTimer(MoveTimer);
	}
	ShieldComponents.Empty();
}




void AtProjectile23::OnNiagaraSystemFinished(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}





void AtProjectile23::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(StartTimer);
	GetWorldTimerManager().ClearTimer(MoveTimer);
	GetWorldTimerManager().ClearTimer(DamageTimer);
	GetWorldTimerManager().ClearTimer(LifeTimer);

	Super::EndPlay(EndPlayReason);
}

