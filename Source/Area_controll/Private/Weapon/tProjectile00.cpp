// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile00.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile00::AtProjectile00()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 1.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(5.0f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Lasert00.NI_Lasert00'")));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAutoActivate(false);
	Niagara->SetAutoDestroy(true);
	Niagara->OnSystemFinished.AddDynamic(this, &AtProjectile00::OnNiagaraSystemFinished);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile00::OnOverlapBegin);

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	Step = 0;
}




void AtProjectile00::SetDamage()
{
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG00_Damage;
	}
	GInstance = nullptr;
}




void AtProjectile00::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	SetDamage();
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			if (IsValid(Sphere))
			{
				if (Step >= 6)
				{
					Niagara->Activate();
					Niagara->SetVariablePosition(FName(TEXT("End")), Sphere->GetComponentLocation());
				}
				Step++;
				Sphere->AddWorldOffset(GetActorForwardVector() * 150.0f, true);
			}
		}, 0.05f, true);
}




void AtProjectile00::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		Sphere->DestroyComponent();
		BoomActor->CreateBoomFunc
		(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT00BoomSystem[0], FColor::White);
		Niagara->Activate();
		Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		Sphere->DestroyComponent();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation
		(OtherActor->GetActorLocation(), SweepResult.Location);
		BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		Niagara->Activate();
		Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				Sphere->DestroyComponent();
				UGameplayStatics::ApplyDamage
				(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				//spown Boom and destroy
				BoomActor->CreateBoomFunc
				(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT00BoomSystem[1], FColor::White);
				Niagara->Activate();
				Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
				return;
			}
		}
	}
}





void AtProjectile00::OnNiagaraSystemFinished(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}





void AtProjectile00::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer0);

	Super::Destroyed();
}
