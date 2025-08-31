// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun03.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/GameplayStatics.h"


ATGun03::ATGun03()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun03.tgun03'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun03.MI_TGun03'"));

	GunMesh->SetMaterial(0, GunMaterial);

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_SphereTP03.NI_SphereTP03'")));
	Niagara->SetAutoActivate(false);

	BoomNSystem = LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT03_obj.NI_BoomT03_obj'"));

	Accurary = 0.0f;

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
}




void ATGun03::Start()
{
	TLCallback.BindUFunction(this, FName("Rotate"));
	TLFreeCallback.BindUFunction(this, FName("FreeRotate"));
	if (CurveFloat)
	{
		RotateTimeLine->AddInterpFloat(CurveFloat, TLCallback);
		FreeRotateTimeLine->AddInterpFloat(CurveFloat, TLFreeCallback);
	}
	TLFinish.BindUFunction(this, FName("FireLogic"));
	RotateTimeLine->SetTimelineFinishedFunc(TLFinish);
	FreeRotateTimeLine->SetPlayRate(0.3f);

	GunRadius->OnComponentBeginOverlap.AddDynamic(this, &ATGun03::On1OverlapBegin);
	

	//timer by rotate and check aim
	GetWorldTimerManager().SetTimer(TimerAim, [this]()
		{
			if (IsValid(AimComponent))
			{
				Tracking();
			}

		}, 0.05f, true, 0.0f);
	GetWorldTimerManager().PauseTimer(TimerAim);

	//timer by firelogic
	GetWorldTimerManager().SetTimer(TimerFire, [this]()
		{
			if (IsShieldOn)
			{
				if (IsValid(Niagara->GetAsset()))
				{
					Niagara->Deactivate();
				}
			}

			if (AimComponents.Num() == 0)
			{
				if (IsValid(Niagara->GetAsset()))
				{
					Niagara->Deactivate();
				}
				
				GunRadius->GetOverlappingComponents(OverlappedComponents);
				if (OverlappedComponents.Num() > 0 && EnemyNames.Num() > 0)
				{
					for (int i = 0; i < OverlappedComponents.Num(); i++)
					{
						for (int k = 0; k < EnemyNames.Num(); k++)
						{
							if (OverlappedComponents.IsValidIndex(i) &&
								IsValid(OverlappedComponents[i]) &&
								OverlappedComponents[i]->ComponentHasTag(EnemyNames[k]))
							{
								AimComponents.AddUnique(OverlappedComponents[i]);
								break;
							}
						}
					}
				}

				OverlappedComponents.Empty();
				GetWorldTimerManager().PauseTimer(TimerAim);
				GetWorldTimerManager().PauseTimer(TimerFire);
				if (AimComponents.Num() > 0)
				{
					for (int i = AimComponents.Num() - 1; i >= 0; --i)
					{
						if (!IsValid(AimComponents[i]) ||
							(IsValid(AimComponents[i]) && ComponentIsFar(AimComponents[i])))
						{
							AimComponents.RemoveAtSwap(i);
						}
					}
					if (AimComponents.Num() > 0)
					{
						GetWorldTimerManager().ClearTimer(TimerFreeRotate);
						FreeRotateTimeLine->Stop();
						StartRotateTimeline();
					}
				}
				else
				{
					TimerElapsed();
				}
			}
			else if (!IsShieldOn)
			{
				Fire();
			}
		}, Gun_Delay, true, 0.3f);
}



void ATGun03::TimerElapsed()
{
}

void ATGun03::Rotate(float Amount)
{
}

void ATGun03::FreeRotate(float Amount)
{
}

void ATGun03::Tracking()
{
}




void ATGun03::StartRotateTimeline()
{
	RotateTimeLine->SetPlayRate(10.0f);
	RotateTimeLine->PlayFromStart();
}




void ATGun03::On1OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				AimComponents.AddUnique(OtherComp);
				if (AimComponents.Num() == 1)
				{
					GetWorldTimerManager().ClearTimer(TimerFreeRotate);
					FreeRotateTimeLine->Stop();
					AimComponent = AimComponents[0];
					CurrentComponentTag = EnemyNames[i];
					GetWorldTimerManager().PauseTimer(TimerAim);
					GetWorldTimerManager().PauseTimer(TimerFire);
					StartRotateTimeline();
					break;
				}
			}
		}
	}
}






void ATGun03::Fire()
{
	if (AimComponents.Num() == 0 || IsShieldOn)
	{
		return;
	}

	if (!Niagara->IsActive())
	{
		Niagara->Activate();
	}

	for (int i = AimComponents.Num() - 1; i >= 0; --i)
	{
		if (AimComponents.IsValidIndex(i) && IsValid(AimComponents[i]) 
			&& IsValid(AimComponents[i]->GetOwner()) && !ComponentIsFar(AimComponents[i]))
		{
			if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice)
			{
				UGameplayStatics::ApplyDamage(AimComponents[i]->GetOwner(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				BoomNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(BoomNSystem, AimComponents[i], NAME_None, FVector::ZeroVector,
						FRotator::ZeroRotator, EAttachLocation::Type::KeepRelativeOffset, true);
				GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
			}
		}
		else
		{
			AimComponents.RemoveAtSwap(i);
		}
	}
}





void ATGun03::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG03_EnergyPrice);
		GunRadius->SetCapsuleRadius(GInstance->TG03_Radius);
		Niagara->SetVariableFloat(TEXT("Radius"), GInstance->TG03_Radius);
		Gun_Delay = GInstance->TG03_GunDelay;
		SpawnDelay = GInstance->TG03_SpawnDelay;
		ShootEnergyPrice = GInstance->TG03_ShootEnergyPrice;
		Damage = GInstance->TG03_Damage;
	}
	GInstance = nullptr;
}