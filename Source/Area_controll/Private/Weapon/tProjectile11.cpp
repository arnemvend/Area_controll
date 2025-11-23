// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile11.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AtProjectile11::AtProjectile11()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 3.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Point = CreateDefaultSubobject<USceneComponent>(TEXT("Point"));
	Point->SetupAttachment(RootComponent);

	MainSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MainCollisionComponent"));
	MainSphere->SetupAttachment(RootComponent);
	MainSphere->SetSphereRadius(3.0f);
	MainSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	MainSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	MainSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	MainSphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile11::OnOverlapBegin);


	NSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_RocketProjt11.NI_RocketProjt11'"));

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");

	
	Speed = 7.2f;
	RotSpeed = 12.0f;
	DestroyTime = 0.1f;
	CanRot = true;
	CanDestroy = true;
}






void AtProjectile11::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG11_Damage;
		RocketNum = GInstance->TG11_RocketNum;
		Expansion = GInstance->TG11_Expansion;
	}
	GInstance = nullptr;

	for (int i = 0; i < RocketNum; i++)
	{
		FName ComponentName = FName(*FString::Printf(TEXT("Sphere_%d"), i));
		SphereArr.Add(NewObject<USphereComponent>(this, ComponentName));
		SphereArr[i]->SetupAttachment(Point);
		SphereArr[i]->RegisterComponent();
		SphereArr[i]->SetSphereRadius(3.0f);
		SphereArr[i]->SetCollisionProfileName(TEXT("OverlapAll"));
		SphereArr[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		SphereArr[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		SphereArr[i]->SetRelativeRotation(UKismetMathLibrary::RandomRotator());
		SphereArr[i]->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile11::OnOverlapBegin);

		NiagaraArr.Add(UNiagaraFunctionLibrary::SpawnSystemAttached
		(NSystem, RootComponent, NAME_None, GetActorLocation(),
			GetActorRotation(), EAttachLocation::Type::KeepRelativeOffset, true));
		NiagaraArr[i]->SetupAttachment(RootComponent);

		Loc.Add(SphereArr[i]->GetComponentLocation());
	}

	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			if (SphereArr.Num() > 0)
			{
				if (CanRot && IsValid(Component))
				{
					SetActorRotation(UKismetMathLibrary::FindLookAtRotation
					(GetActorLocation(), Component->GetComponentLocation()));
					Point->AddRelativeRotation(FRotator(0.0f, 0.0f, RotSpeed));
					AddActorWorldOffset(FVector(GetActorForwardVector() * Speed), true);
				}
				else
				{
					Point->AddRelativeRotation(FRotator(0.0f, 0.0f, RotSpeed));
					AddActorWorldOffset(FVector(GetActorForwardVector() * Speed), true);
					if (CanDestroy)
					{
						CanDestroy = false;
						EndOfWay();
					}
				}

				if (SphereArr.Num() > 0)
				{
					for (int i = SphereArr.Num() - 1; i >= 0; --i)
					{
						if (SphereArr.IsValidIndex(i) && IsValid(SphereArr[i]))
						{
							SphereArr[i]->AddWorldOffset(SphereArr[i]->GetForwardVector() * Expansion, true);
						}
						if (SphereArr.IsValidIndex(i) && IsValid(SphereArr[i]))
						{
							FVector NewLocation = SphereArr[i]->GetComponentLocation();

							if (NiagaraArr.IsValidIndex(i) && IsValid(NiagaraArr[i]) && Loc.IsValidIndex(i))
							{
								NiagaraArr[i]->SetWorldLocation(NewLocation);
								NiagaraArr[i]->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(Loc[i], NewLocation));
								Loc[i] = NewLocation;
							}
						}
					}
				}
			}
			else
			{
				Destroy();
			}
		}, 0.04f, true);
}





void AtProjectile11::DeleteFunc(int a)
{
	NiagaraArr[a]->DeactivateImmediate();
	NiagaraArr[a]->DestroyInstance();
	NiagaraArr.RemoveAt(a);
	SphereArr[a]->OnComponentBeginOverlap.Clear();
	SphereArr[a]->DestroyComponent();
	SphereArr.RemoveAt(a);
	Loc.RemoveAt(a);
}




void AtProjectile11::EndOfWay()
{
	GetWorldTimerManager().SetTimer(Timer1, [this]()
		{
			if (SphereArr.Num() > 0)
			{
				int a = SphereArr.Num() - 1;
				if (IsValid(SphereArr[a]))
				{
					BoomActor->CreateBoomFunc(SphereArr[a]->GetComponentLocation(),
						FRotator::ZeroRotator, BoomActor->ProjT10BoomSystem, FColor::White);
					DeleteFunc(a);
				}
				EndOfWay();
			}
			else
			{
				Destroy();
			}
		}, DestroyTime, false);
}





void AtProjectile11::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);

	if (SphereArr.Num() > 0)
	{
		for (int n = SphereArr.Num(); n >= 0; --n)
		{
			if (SphereArr.IsValidIndex(n) && IsValid(SphereArr[n]))
			{
				SphereArr[n]->OnComponentBeginOverlap.Clear();
				SphereArr[n]->DestroyComponent();
				SphereArr.RemoveAt(n);
			}
		}
	}
	if (NiagaraArr.Num() > 0)
	{
		for (int m = SphereArr.Num(); m >= 0; --m)
		{
			if (NiagaraArr.IsValidIndex(m) && IsValid(NiagaraArr[m]))
			{
				NiagaraArr[m]->DeactivateImmediate();
				NiagaraArr[m]->DestroyInstance();
				NiagaraArr.RemoveAt(m);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}





void AtProjectile11::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(OtherComp) || OtherComp->ComponentTags.Num() == 0)
	{
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		for (int i = 0; i < SphereArr.Num(); i++)
		{
			if (IsValid(SphereArr[i]) && IsValid(OverlappedComp) && SphereArr[i] == OverlappedComp)
			{
				BoomActor->CreateBoomFunc
				(SweepResult.Location, FRotator::ZeroRotator, BoomActor->Proj0BoomSystem[1], FColor::White);
				DeleteFunc(i);
				break;
			}
		}
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		for (int i = 0; i < SphereArr.Num(); i++)
		{
			if (IsValid(SphereArr[i]) && IsValid(OverlappedComp) && SphereArr[i] == OverlappedComp)
			{
				const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation
				(OtherActor->GetActorLocation(), SweepResult.Location);
				BoomActor->CreateBoomFunc
				(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
				DeleteFunc(i);
				break;
			}
		}
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				if (IsValid(MainSphere) 
					&& IsValid(OverlappedComp) 
					&& MainSphere == OverlappedComp
					&& OtherComp == Component)
				{
					CanRot = false;
					EndOfWay();
					return;
				}
				for (int k = SphereArr.Num() - 1; k >= 0; k--)
				{
					if (IsValid(SphereArr[k]) && IsValid(OverlappedComp) && SphereArr[k] == OverlappedComp)
					{
						BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), 
							FRotator::ZeroRotator, BoomActor->ProjT10BoomSystem, FColor::White);
						UGameplayStatics::ApplyDamage
						(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
						DeleteFunc(k);
						break;
					}
				}
				return;
			}
		}
	}
}
