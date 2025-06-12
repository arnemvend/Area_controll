// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Gun.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"


// Sets default values
AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	GunRadius = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	GunRadius->SetupAttachment(RootComponent);
	GunRadius->SetCapsuleHalfHeight(1000.0f);
	GunRadius->SetCapsuleRadius(1.0f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetAutoActivate(false);

	SpawnNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnNiagaraComponent"));
	SpawnNiagara->SetupAttachment(GunMesh);
	SpawnNiagara->SetAutoActivate(false);
	SpawnNiagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Builder.NI_Builder'")));

	RotateTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLine"));
	RotateTimeLine->SetLooping(false);
	RotateTimeLine->SetComponentTickInterval(0.04f);

	FreeRotateTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("FreeTimeLine"));
	FreeRotateTimeLine->SetLooping(false);
	FreeRotateTimeLine->SetComponentTickInterval(0.04f);
	
	

	

	FreeInterval = 5.0f;

	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("NeutralInternal");

	AimComponent = nullptr;

	V2Index = 5.0f;
	OpasityPower2 = 2.0f;
	IsShieldOn = false;
	IsFirst = true;
}




//set parameters from structure
void AGun::Start()
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
	RotateTimeLine->SetPlayRate(0.4f);
	FreeRotateTimeLine->SetPlayRate(0.4f);

	GunRadius->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnOverlapBegin);

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
			if (!IsValid(AimComponent)
				|| (IsValid(AimComponent) && ComponentIsFar(AimComponent))
				|| (IsValid(AimComponent) && AimComponent->ComponentTags[0] != CurrentComponentTag))
			{
				Niagara->DeactivateImmediate();
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
				AimComponents.RemoveSwap(AimComponent);
				AimComponent = nullptr;
				if (AimComponents.Num() > 0)
				{
					float Distance = 5000.0;
					for (int i = AimComponents.Num() - 1; i >= 0; --i)
					{
						if (!IsValid(AimComponents[i]) ||
							(IsValid(AimComponents[i]) && ComponentIsFar(AimComponents[i])))
						{
							AimComponents.RemoveAtSwap(i);
						}
						else
						{
							const float Dist = HorizontalDistance(GetActorLocation(), AimComponents[i]->GetComponentLocation());
							if (Dist < Distance)
							{
								AimComponent = AimComponents[i];
								CurrentComponentTag = AimComponents[i]->ComponentTags[0];
								Distance = Dist;
							}
						}
					}
					if (AimComponents.Num() > 0 && IsValid(AimComponent))
					{
						GetWorldTimerManager().ClearTimer(TimerFreeRotate);
						FreeRotateTimeLine->Stop();
						RotateTimeLine->PlayFromStart();
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
	}, Gun_Delay, true, 0.2f);
}



void AGun::Tracking()
{
	Rot = UKismetMathLibrary::FindLookAtRotation
	(GetActorLocation(), AimComponent->GetComponentLocation());
	SetActorRotation(Rot);
}





//logic of rotate
void AGun::Rotate(float Amount)
{
	if (IsValid(AimComponent))
	{
		Rot = UKismetMathLibrary::FindLookAtRotation
		(GetActorLocation(), AimComponent->GetComponentLocation());
	}
}



void AGun::FreeRotate(float Amount)
{
	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, FreeAngle, 0.0f), Amount, true));
}



void AGun::FireLogic()
{
	Niagara->Activate();
	if (GetWorldTimerManager().IsTimerPaused(TimerAim))
	{
		GetWorldTimerManager().UnPauseTimer(TimerAim);
	}
	if (GetWorldTimerManager().IsTimerPaused(TimerFire))
	{
		GetWorldTimerManager().UnPauseTimer(TimerFire);
	}
}




//spawn projectile
void AGun::Fire()
{
	FRotator Rotate;
	Rotate.Roll = GetActorRotation().Roll + FMath::FRandRange(Accurary * (-1.0f), Accurary);
	Rotate.Yaw = GetActorRotation().Yaw + FMath::FRandRange(Accurary * (-1.0f), Accurary);
	Rotate.Pitch = GetActorRotation().Pitch + FMath::FRandRange(Accurary * (-1.0f), Accurary);
	Prj = GetWorld()->SpawnActor<AProjectile>(SpownProjectile, Niagara->GetComponentLocation(), Rotate);
}





bool AGun::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 14.0));
}





float AGun::HorizontalDistance(FVector A, FVector B)
{
	return FVector2D(A.X - B.X, A.Y - B.Y).Length();
}




void AGun::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
					RotateTimeLine->PlayFromStart();
					break;
				}
			}
		}
	}
}



void AGun::TimerElapsed()
{
	FreeInterval = FMath::FRandRange(4.0f, 10.0f);
	FreeAngle = FMath::FRandRange(0.0f, 360.0f);
	GetWorldTimerManager().SetTimer(TimerFreeRotate, [this]()
		{
			if (AimComponents.Num() == 0)
			{
				FreeRotateTimeLine->PlayFromStart();
			}
			TimerElapsed();
		}, FreeInterval, false);
}




void AGun::ColorFunc(FColor Color)
{
	if (!GunMesh->GetStaticMesh() && !GunMesh->GetStaticMesh()->GetMaterial(0))
	{
		return;
	}

	if (IsFirst)
	{
		IsFirst = false;
		DMaterial = GunMesh->CreateDynamicMaterialInstance(0, GunMaterial);
		if (IsValid(DMaterial))
		{
			DMaterial->GetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEM);
		}
	}
	if (IsValid(DMaterial))
	{
		DMaterial->SetVectorParameterValue(TEXT("LightParam"), Color);
		DMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEM);
	}
}



void AGun::SpawnFunc(FColor Color)
{
	if (IsValid(GunMesh) && IsValid(GunMesh->GetStaticMesh()))
	{
		ColorFunc(Color);
		SpawnNiagara->Activate();
		SpawnNiagara->SetVariableLinearColor(TEXT("Color"), Color);
		SpawnNiagara->SetVariableStaticMesh(TEXT("SousceMesh"), GunMesh->GetStaticMesh());
		SpawnNiagara->SetVariableFloat(TEXT("V2Index"), V2Index);
		SpawnNiagara->SetVariableFloat(TEXT("OpasityPower2"), OpasityPower2);
		
		GetWorldTimerManager().SetTimer(TimerSpawn, [this]()
			{
				SpawnNiagara->DeactivateImmediate();
				TimerElapsed();
				Start();
			}, SpawnDelay, false);
	}
}



void AGun::DeleteFunc()
{
	SpawnNiagara->Activate();
	GetWorldTimerManager().SetTimer(TimerSpawn, [this]()
		{
			SpawnNiagara->DeactivateImmediate();
			Destroy();
		}, 0.5f, false);
}



void AGun::Stop()
{
	GunRadius->OnComponentBeginOverlap.RemoveDynamic(this, &AGun::OnOverlapBegin);

	RotateTimeLine->Stop();
	FreeRotateTimeLine->Stop();

	GetWorldTimerManager().ClearTimer(TimerAim);
	GetWorldTimerManager().ClearTimer(TimerFire);
	GetWorldTimerManager().ClearTimer(TimerFreeRotate);
	GetWorldTimerManager().ClearTimer(TimerSpawn);

	Niagara->DeactivateImmediate();
	SpawnNiagara->DeactivateImmediate();

	AimComponents.Empty(0);
	OverlappedComponents.Empty(0);
	AimComponent = nullptr;
	IsShieldOn = false;
}




// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
}






// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGun::Destroyed()
{
	GetWorldTimerManager().ClearTimer(TimerAim);
	GetWorldTimerManager().ClearTimer(TimerFire);
	GetWorldTimerManager().ClearTimer(TimerFreeRotate);
	GetWorldTimerManager().ClearTimer(TimerSpawn);

	Super::Destroyed();


}

