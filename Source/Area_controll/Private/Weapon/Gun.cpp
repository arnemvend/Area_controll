// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Gun.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Structure/StructWild.h"
//#include "Weapon/Projectile_0.h"


// Sets default values
AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	GunRadius = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	GunRadius->SetupAttachment(RootComponent);
	GunRadius->SetCapsuleHalfHeight(600.0f);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(RootComponent);
	Niagara->SetActive(false);

	RotateTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLine"));
	RotateTimeLine->SetLooping(false);
	RotateTimeLine->SetComponentTickInterval(0.04f);

	Type = 0;
	CanStartTimer = false;

	GunRadius->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnOverlapBegin);
	GunRadius->OnComponentEndOverlap.AddDynamic(this, &AGun::OnOverlapEnd);
}




//set parameters from structure
void AGun::Start()
{
	FStructWild StructWild;
	GunMesh->SetStaticMesh(StructWild.Gun_SM[Type]);
	GunMesh->SetMaterial(0, StructWild.Gun_MI[Type]);
	GunRadius->SetCapsuleRadius(StructWild.Gun_Radius[Type]);
	Niagara->SetAsset(StructWild.Gun_NS[Type]);
	Accurary = StructWild.Gun_Accurary[Type];
	Gun_Speed = StructWild.Gun_Speed[Type];
	SpownProjectile = StructWild.Gun_BP[Type];

	//timer by firelogic
	GetWorldTimerManager().SetTimer(TimerFire, [this]()
	{
		if (CanStartTimer)
		{
			Fire();
		}
	}, Gun_Speed, true);
	GetWorldTimerManager().PauseTimer(TimerFire);

	//timer by rotate and check aim
	GetWorldTimerManager().SetTimer(TimerAim, [this]()
	{
		if (CanStartTimer)
		{
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AimLoc));
		    if (IsValid(AimActor) == false)
		    {
			    Niagara->DeactivateImmediate();
			    GetWorldTimerManager().PauseTimer(TimerAim);
			    GetWorldTimerManager().PauseTimer(TimerFire);
			    Aims.Remove(AimActor);
				AimActor = nullptr;
			    if (Aims.Num() > 0)
			    {
					AimActor = Aims[0];
				    Find();
			    }
		    }
		}
		
	}, 0.04f, true);
	GetWorldTimerManager().PauseTimer(TimerAim);
	CanStartTimer = true;
}


//Logic of find new aim------------------------------>
void AGun::Find()
{
	if (AimActor)
	{
		AimLoc = AimActor->GetActorLocation();
		RotateTimeLine->PlayFromStart();
	}
}



//logic of rotate
void AGun::Rotate(float Amount)
{
	Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AimLoc);
	SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), Rot, Amount, false));
}


void AGun::FireLogic()
{
	Niagara->Activate();
	Fire();
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
	GetWorld()->SpawnActor<AActor>(SpownProjectile, GetActorLocation(), Rotate);
}



void AGun::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && 
		(OtherComp->ComponentHasTag(TEXT("Internal")) || 
		OtherComp->ComponentHasTag(TEXT("InternalEnemy"))))
	{
		Aims.AddUnique(OtherActor);
		if (Aims.Num() == 1)
		{
			AimActor = Aims[0];
			GetWorldTimerManager().PauseTimer(TimerAim);
			GetWorldTimerManager().PauseTimer(TimerFire);
			Find();
		}
	}
}

void AGun::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Aims.Num() > 0)
	{
		if (AimActor == OtherActor)
		{
			Niagara->DeactivateImmediate();
			GetWorldTimerManager().PauseTimer(TimerAim);
			GetWorldTimerManager().PauseTimer(TimerFire);
			Aims.Remove(AimActor);
			AimActor = nullptr;
			if (Aims.Num() > 0)
			{
				if (Aims.Num() > 0)
				{
					AimActor = Aims[0];
					Find();
				}
			}
		}
		else
		{
			if (OtherComp->ComponentHasTag(TEXT("Internal")) || OtherComp->ComponentHasTag(TEXT("InternalEnemy")))
			{
				Aims.Remove(OtherActor);
			}
		}
	}
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	TLCallback.BindUFunction(this, FName("Rotate"));
	if (CurveFloat)
	{
		RotateTimeLine->AddInterpFloat(CurveFloat, TLCallback);
	}
	TLFinish.BindUFunction(this, FName("FireLogic"));
	RotateTimeLine->SetTimelineFinishedFunc(TLFinish);
	RotateTimeLine->SetPlayRate(0.5f);

	

}




// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGun::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(TimerAim);
	GetWorldTimerManager().ClearTimer(TimerFire);
}

