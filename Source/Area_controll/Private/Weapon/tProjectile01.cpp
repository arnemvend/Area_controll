// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile01.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile01::AtProjectile01()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 5.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(2.0f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_DarkProjt01.NI_DarkProjt01'")));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAutoActivate(true);


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");


	Speed = 0.64f;
	Jump = 0;
	RealyScale = Sphere->GetScaledSphereRadius();
	DeathDelay = 0.5f;


	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile01::OnOverlapBegin);
}







void AtProjectile01::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG01_Damage;
		MaxJump = GInstance->TG01_MaxJump;
		JumpRadius = GInstance->TG01_JumpRadius;
	}
	GInstance = nullptr;

	//timer for move
	GetWorldTimerManager().SetTimer(Timer, [this]()
		{
			if (IsValid(Component))
			{
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation
				(GetActorLocation(), Component->GetComponentLocation()));
				AddActorWorldOffset(FVector(GetActorForwardVector() * Speed), true);
			}
			else
			{
				JumpFunc();
			}
		}, 0.04f, true);
}




void AtProjectile01::JumpFunc()
{
	OverlappedComponents.Empty();
	GetWorldTimerManager().PauseTimer(Timer);
	Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &AtProjectile01::OnOverlapBegin);
	if (IsValid(Component))
	{
		CallBoom(Component->GetComponentLocation());
		if (Jump >= MaxJump)
		{
			DeathFunc();
			return;
		}
	}
	float Distance = 2000.0f;
	FVector2D Loc1;
	FVector2D Loc2;
	Loc1.X = Sphere->GetComponentLocation().X;
	Loc1.Y = Sphere->GetComponentLocation().Y;
	Sphere->SetSphereRadius(JumpRadius);
	Sphere->GetOverlappingComponents(OverlappedComponents);
	OverlappedComponents.RemoveSwap(Component);
	Component = nullptr;
	if (OverlappedComponents.Num() > 0 && EnemyNames.Num() > 0)
	{
		for (int i = 0; i < OverlappedComponents.Num(); i++)
		{
			for (int k = 0; k < EnemyNames.Num(); k++)
			{
				if (OverlappedComponents[i]->ComponentHasTag(EnemyNames[k]))
				{
					Loc2.X = OverlappedComponents[i]->GetComponentLocation().X;
					Loc2.Y = OverlappedComponents[i]->GetComponentLocation().Y;
					float D = FVector2D(Loc1 - Loc2).Length();
					if (D < Distance)
					{
						Component = OverlappedComponents[i];
						Distance = D;
					}
					break;
				}
			}
		}
		if (IsValid(Component))
		{
			Sphere->SetSphereRadius(RealyScale);
			Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile01::OnOverlapBegin);
			GetWorldTimerManager().UnPauseTimer(Timer);
		}
		else
		{
			CallBoom(GetActorLocation());
			DeathFunc();
		}
	}
	else
	{
		CallBoom(GetActorLocation());
		DeathFunc();
	}
	Jump++;
}




void AtProjectile01::DeathFunc()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
		{
			Destroy();
		}, DeathDelay, false);
}



void AtProjectile01::CallBoom(FVector Loc)
{
	BoomActor->CreateBoomFunc
	(Loc, FRotator::ZeroRotator, BoomActor->ProjT01BoomSystem, FColor::White);
}






void AtProjectile01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




void AtProjectile01::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().ClearTimer(DestroyTimer);


	Super::Destroyed();
}




void AtProjectile01::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		GetWorldTimerManager().PauseTimer(Timer);
		BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[1], FColor::White);
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &AtProjectile01::OnOverlapBegin);
		DeathFunc();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		GetWorldTimerManager().PauseTimer(Timer);
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation
		(OtherActor->GetActorLocation(), GetActorLocation());
		BoomActor->CreateBoomFunc(GetActorLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &AtProjectile01::OnOverlapBegin);
		DeathFunc();
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				UGameplayStatics::ApplyDamage
				(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				JumpFunc();
				return;
			}
		}
	}
}

