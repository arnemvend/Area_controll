// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile21.h"
#include "Kismet/GameplayStatics.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile21::AtProjectile21()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 3.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	/*Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(4.0f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile21::OnOverlapBegin);*/

	NSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ClasterBomb.NI_ClasterBomb'"));
	NiagaraStart = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraClasterBom"));
	NiagaraStart->SetupAttachment(RootComponent);
	NiagaraStart->SetAsset(NSystem);

	NSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Claster.NI_Claster'"));

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");

	Step = 0;
	Speed = 20.0f;
	FreeFall = 2.0f;
	CanStart = true;
}



void AtProjectile21::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG21_Damage;
		TimeStep = GInstance->TG21_TimeStep;
		LenghtExpl = GInstance->TG21_LenghtExpl;
	}
	GInstance = nullptr;

	CurrentSpeed = GetActorForwardVector() * Speed;

	GetWorldTimerManager().SetTimer(Timer, [this]()
		{
			CurrentSpeed.Z = CurrentSpeed.Z - (FreeFall * Step);
			AddActorWorldOffset(CurrentSpeed);
			Step++;
			if (HorizontalDistance(Aim, GetActorLocation()) <= LenghtExpl && CanStart)
			{
				CanStart = false;
				MoveToDown();
			}
			if (HorizontalDistance(Aim, GetActorLocation()) >= LenghtExpl && !CanStart)
			{
				Destroy();
			}
		}, 0.04f, true);
}




float AtProjectile21::HorizontalDistance(FVector A, FVector B)
{
	return FVector2D(A.X - B.X, A.Y - B.Y).Length();
}





void AtProjectile21::MoveToDown()
{
	GetWorldTimerManager().SetTimer(Timer1, [this]()
		{
			Sphere = NewObject<USphereComponent>(this, "Sphere");
			Sphere->SetupAttachment(RootComponent);
			Sphere->RegisterComponent();
			Sphere->SetSphereRadius(4.0f);
			Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
			Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			Sphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
			Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile21::OnOverlapBegin);

			GetWorldTimerManager().SetTimer(Timer2, [this]()
				{
					Sphere->AddWorldOffset(FVector(0.0f, 0.0f, -150.0f), true);
				}, 0.03f, true);

		}, TimeStep, true, 0.01f);
}




void AtProjectile21::SparkFunc(FVector Loc)
{
	Sphere->DestroyComponent();
	GetWorldTimerManager().PauseTimer(Timer2);
	NiagaraDamage = UNiagaraFunctionLibrary::SpawnSystemAttached
	(NSystem, RootComponent, NAME_None, GetActorLocation(),
		GetActorRotation(), EAttachLocation::Type::KeepRelativeOffset, true);
	NiagaraDamage->SetVariablePosition(FName(TEXT("Start")), GetActorLocation());
	NiagaraDamage->SetVariablePosition(FName(TEXT("EndVector")), Loc);
}





void AtProjectile21::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(OtherComp) || OtherComp->ComponentTags.Num() == 0)
	{
		return;
	}
	if (SweepResult.Location == FVector::ZeroVector)
	{
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		SparkFunc(SweepResult.Location);
		BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj1BoomSystem[0], FColor::White);
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		SparkFunc(SweepResult.Location);
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				SparkFunc(SweepResult.Location);
				UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj1BoomSystem[1], FColor::White);
				return;
			}
		}
	}
}





void AtProjectile21::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().ClearTimer(Timer1);
	GetWorldTimerManager().ClearTimer(Timer2);

	Super::EndPlay(EndPlayReason);
}






