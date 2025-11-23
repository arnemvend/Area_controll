// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tGun13.h"
#include "Wild.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/GameplayStatics.h"




AtGun13::AtGun13()
{
	GunMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/tgun13.tgun13'")));

	GunMaterial = LoadObject<UMaterialInterface>
		(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_TGun13.MI_TGun13'"));

	GunMesh->SetMaterial(0, GunMaterial);

	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_SphereTP13.NI_SphereTP13'")));
	Niagara->SetAutoActivate(false);

	BoomNSystem = LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT13_obj.NI_BoomT13_obj'"));

	Accurary = 0.0f;
	IsFirstCheck = true;

	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
}




void AtGun13::Start()
{
	if (IsFirst)
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

		IsFirst = false;
	}

	GunRadius->OnComponentBeginOverlap.AddUniqueDynamic(this, &AtGun13::OnOverlapBegin);


	//timer by rotate and check aim
	GetWorldTimerManager().SetTimer(TimerAim, [this]()
		{
			if (IsValid(AimComponent))
			{
				Tracking();
			}

		}, 0.05f, false, 0.0f);
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

			if (AimComponents.Num() == 0 || IsFirstCheck)
			{
				IsFirstCheck = false;

				if (IsValid(Niagara->GetAsset()))
				{
					Niagara->Deactivate();
				}

				GunRadius->GetOverlappingComponents(OverlappedComponents);
				if (OverlappedComponents.Num() > 0 && EnemyNames.Num() > 0)
				{
					for (int n = OverlappedComponents.Num() - 1; n >= 0; --n)
					{
						if (IsValid(OverlappedComponents[n]) && OverlappedComponents[n]->ComponentTags.Num() == 0)
						{
							OverlappedComponents.RemoveAtSwap(n);
						}
					}
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




void AtGun13::TimerElapsed()
{
}

void AtGun13::Rotate(float Amount)
{
}

void AtGun13::FreeRotate(float Amount)
{
}

void AtGun13::Tracking()
{
}




void AtGun13::StartRotateTimeline()
{
	RotateTimeLine->SetPlayRate(10.0f);
	RotateTimeLine->PlayFromStart();
}



void AtGun13::OnOverlapLogic(UPrimitiveComponent* OtherComp)
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


/*void AtGun13::On1OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}*/





void AtGun13::Fire()
{
	if (AimComponents.Num() == 0)
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
			AWild* Wild = Cast<AWild>(AimComponents[i]->GetOwner());
			if (IsValid(GMode) && GMode->PlayerEnergy >= ShootEnergyPrice && IsValid(Wild))
			{
				const FVector Position = FVector(AimComponents[i]->GetComponentLocation().X, AimComponents[i]->GetComponentLocation().Y, 5.0f);
				//ComponentPosition = FVector(0.0f, 0.0f, AimComponents[i]->GetComponentLocation().Z);
				Wild->DownDpeed = DownSpeed;
				Wild->DownTime = DownTime;
				Wild->MoveDownStart();
				UGameplayStatics::ApplyDamage(Wild, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				BoomNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(BoomNSystem, AimComponents[i], NAME_None, AimComponents[i]->GetComponentLocation(),
					FRotator::ZeroRotator, EAttachLocation::Type::KeepWorldPosition, true);
				BoomNiagara->SetVariablePosition(TEXT("Position"), Position);
				GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - ShootEnergyPrice);
			}
			Wild = nullptr;
		}
		else
		{
			AimComponents.RemoveAtSwap(i);
		}
	}
}





void AtGun13::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GInstance) && IsValid(GMode))
	{
		GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - GInstance->TG13_EnergyPrice);
		GunRadius->SetCapsuleRadius(GInstance->TG13_Radius);
		Niagara->SetVariableFloat(TEXT("Radius"), GInstance->TG13_Radius);
		Gun_Delay = GInstance->TG13_GunDelay;
		SpawnDelay = GInstance->TG13_SpawnDelay;
		ShootEnergyPrice = GInstance->TG13_ShootEnergyPrice;
		Damage = GInstance->TG13_Damage;
		DownSpeed = GInstance->TG13_DownSpeed;
		DownTime = GInstance->TG13_DownTime;
	}
	GInstance = nullptr;
}