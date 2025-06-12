// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile20.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile20::AtProjectile20()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InitialLifeSpan = 8.0f;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(10.0f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);


	NiagaraStart = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraStart"));
	NiagaraStart->SetupAttachment(RootComponent);
	NiagaraStart->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BigEndT20.NI_BigEndT20'")));
	NiagaraStart->SetAutoActivate(true);


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");


	DelayTime = 3.0f;
	IsFall = false;
}




void AtProjectile20::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG20_Damage;
		ExplRadius = GInstance->TG20_ExplRadius;
	}
	GInstance = nullptr;

	GetWorldTimerManager().SetTimer(Timer, [this]()
		{
			SetActorLocation(FVector(Aim.X, Aim.Y, 300.0f));
			SetActorRotation
			(FRotator(0.0f, UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f), 0.0f));
			Sphere->AddRelativeLocation
			(FVector(UKismetMathLibrary::RandomFloatInRange(0.0f, Aim.Z), 0.0f, 0.0f));
			Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile20::OnOverlapBegin);
			NiagaraStart->DeactivateImmediate();
			NiagaraStart->OnSystemFinished.AddDynamic(this, &AtProjectile20::OnNiagaraFinished);
			
			GetWorldTimerManager().SetTimer(Timer2, [this]()
				{
				    if (GetActorLocation().Z > 0.1)
				    {
						Sphere->AddWorldOffset(FVector(0.0f, 0.0f, -100.0f), true);
				    }
					else
					{
						GetWorldTimerManager().ClearTimer(Timer2);
					}
				},0.05, true);
		}, DelayTime, false);
}






void AtProjectile20::OnNiagaraFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy();
}




void AtProjectile20::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().ClearTimer(Timer1);
	GetWorldTimerManager().ClearTimer(Timer2);

	Super::Destroyed();
}





float AtProjectile20::HorizontalDistance(FVector A, FVector B)
{
	return FVector2D(A.X - B.X, A.Y - B.Y).Length();
}




void AtProjectile20::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		GetWorldTimerManager().ClearTimer(Timer2);
		FVector Location = SweepResult.Location;
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation
		(OtherActor->GetActorLocation(), Location);
		Sphere->OnComponentBeginOverlap.Clear();
		if (IsFall)
		{
			BoomActor->CreateBoomFunc(Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		else
		{
			Sphere->SetWorldLocation(Location);
			NiagaraStart->SetWorldLocation(Sphere->GetComponentLocation());
			NiagaraStart->Activate();
			UE_LOG(LogTemp, Warning, TEXT("%f"), Location.Z);
			GetWorldTimerManager().SetTimer(Timer1, [this, Location, SpawnRotation]()
				{
					BoomActor->CreateBoomFunc(Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
				}, 0.05f, true);
		}
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		IsFall = true;
		GetWorldTimerManager().ClearTimer(Timer2);
		FVector Location = SweepResult.Location;
		Location.Z = 0.1f;
		NiagaraStart->SetWorldLocation(Location);
		NiagaraStart->Activate();
		Sphere->SetSphereRadius(ExplRadius);
		BoomActor->CreateBoomFunc(Location, FRotator::ZeroRotator, BoomActor->ProjT20BoomSystem, FColor::White);
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				if (IsFall)
				{
					float Amount = ExplRadius - HorizontalDistance(Sphere->GetComponentLocation(), OtherComp->GetComponentLocation());
					if (Amount < 0)
					{
						Amount = 0;
					}
					Amount = Amount / ExplRadius;
					float CurrentDamage = UKismetMathLibrary::Lerp(0.0f, Damage, Amount);
					UGameplayStatics::ApplyDamage
					(OtherActor, CurrentDamage, GetInstigatorController(), this, UDamageType::StaticClass());
					return;
				}
				else
				{
					UGameplayStatics::ApplyDamage
					(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
					return;
				}
			}
		}
	}
}
