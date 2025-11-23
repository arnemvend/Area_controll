// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Wild.h"
#include "Boom.h"
//#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Gun.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AWild::AWild()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);

	MeshShadow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	MeshShadow->SetupAttachment(RootComponent);
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'"));
	MeshShadow->SetStaticMesh(MyStaticMesh);
	MeshShadow->SetMaterial(0, LoadObject<UMaterialInterface>
		(nullptr, TEXT("MaterialInstanceConstant'/Game/Weapon/Materials/MI_Shadow_Light.MI_Shadow_Light'")));
	
	MeshShadow->SetVisibility(false);
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Box->SetupAttachment(Mesh);
	Box->ComponentTags.Add("WildLighter");

	Nose = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerNose"));
	Nose->SetupAttachment(Mesh);

	Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	Gun->SetupAttachment(Mesh);
	Gun->SetVisibility(false);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	Spline->AddSplinePointAtIndex(FVector::ZeroVector, 0, ESplineCoordinateSpace::Local);
	Spline->AddSplinePointAtIndex(FVector(100.0f, 0.0f, 0.0f), 1, ESplineCoordinateSpace::Local);
    Spline->AddSplinePointAtIndex(FVector(200.0f, 0.0f, 0.0f), 2, ESplineCoordinateSpace::Local);
	Spline->ReparamStepsPerSegment = 2;
	Spline->bSplineHasBeenEdited = false;

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWild::OnBoxOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AWild::OnBoxOverlapEnd);
	Nose->OnComponentEndOverlap.AddDynamic(this, &AWild::OnNoseOverlapEnd);

	OnTakeAnyDamage.AddDynamic(this, &AWild::OnTakeDamage);

	CanDamage = false;
	CanDown = false;
	CustomDeltaTime = 0.0333f;
	CreationTime = 3.0f;
	CurrentCreateValue = 0.0f;
	CurrentMoveNumber = 0;
	EndUp = 0;
}



void AWild::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Health > 0.0f && CanDamage)
	{
		Health = Health - Damage;
		if (Health <= 0.0f)
		{
			Destroy();
			return;
		}
		float const Alpha = FMath::Max(0.1f, Health / MaxHealth);
		if (DMeshMaterial)
		{
			DMeshMaterial->SetScalarParameterValue(TEXT("Damage"), Alpha);
		}
	}
}




//set start parameters from StructWild
void AWild::Start()
{
	DMeshMaterial = Mesh->CreateDynamicMaterialInstance(0, MeshMaterial);
	DMeshMaterial->SetVectorParameterValue(TEXT("Emissive"), Color);

	GetWorldTimerManager().SetTimer(CreateTimer, [this]()
		{
			if (CurrentCreateValue != CreationTime)
			{
				DMeshMaterial->SetScalarParameterValue(TEXT("Amount"), 1 - (CurrentCreateValue / CreationTime));
				CurrentCreateValue = FMath::Min(CreationTime, CurrentCreateValue + CustomDeltaTime);
			}
			else
			{
				Continue();
				if (CreateTimer.IsValid() && GetWorldTimerManager().TimerExists(CreateTimer))
				{
					GetWorldTimerManager().ClearTimer(CreateTimer);
				}
			}
		}, CustomDeltaTime, true);
}






void AWild::MoveDownStart()
{
	if (!CanDown)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(DownTimer, [this]()
		{
			if (IsValid(Mesh))
			{
				Mesh->AddWorldOffset(FVector(0.0f, 0.0f, DownDpeed));
			}
		}, CustomDeltaTime, true);

	GetWorldTimerManager().SetTimer(DownStop, [this]()
		{
			MoveDownStop();
		}, DownTime, false);
}


void AWild::MoveDownStop()
{
	if (MoveTimer.IsValid() && GetWorldTimerManager().IsTimerActive(DownTimer))
	{
		GetWorldTimerManager().PauseTimer(DownTimer);
	}
}








//setting the trajectory in spline local space
void AWild::Continue()
{
	Gun->SetVisibility(true);
	AGun* GunActor = Cast<AGun>(Gun->GetChildActor());
	//GunActor->Color = Color;
	GunActor->TimerElapsed();
	GunActor->Start();
	GunActor = nullptr;
	
	FVector Loc;
	Loc.X = Mesh->GetComponentLocation().X;
	Loc.Y = Mesh->GetComponentLocation().Y;
	Loc.Z = 0.1f;
	MeshShadow->SetWorldLocation(Loc);
	MeshShadow->SetVisibility(true);
	//set coordinates 2 point
	Loc.X = AimCoord.X + UKismetMathLibrary::RandomFloatInRange(-150.0f, 150.0f);
	Loc.Y = AimCoord.Y + UKismetMathLibrary::RandomFloatInRange(-150.0f, 150.0f);
	Loc.Z = GetActorLocation().Z;
	Spline->SetLocationAtSplinePoint(2, Loc, ESplineCoordinateSpace::World, true);
	//set coordinates 1 point
	Loc = Spline->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::Local) * 0.5f;
	Loc.X = Loc.X + UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
	Loc.Y = Loc.Y + UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
	Spline->SetLocationAtSplinePoint(1, Loc, ESplineCoordinateSpace::Local, true);

	High = High + UKismetMathLibrary::RandomFloatInRange(-20.0f, 20.0f);

	const float TimeOfWay = Spline->GetSplineLength() / Speed;
	const float RelativeTime = CustomDeltaTime / TimeOfWay;
	if (!(CurveFloat1 && IsValid(Spline)))
	{
		return;
	}
	for (float Fi = 0.0f; Fi <= 1.0f; Fi += RelativeTime)
	{
		LocArr.Add(Spline->GetLocationAtTime(Fi, ESplineCoordinateSpace::World, true));
		if (LocArr.Num() > 0)
		{
			if (CurveFloat1->GetFloatValue(Fi) < 1.0f)
			{
				LocArr.Last() = FVector(LocArr.Last().X, LocArr.Last().Y, FMath::Lerp(GetActorLocation().Z, High, CurveFloat1->GetFloatValue(Fi)));
			}
			else
			{
				LocArr.Last() = FVector(LocArr.Last().X, LocArr.Last().Y, High);
				if (EndUp == 0)
				{
					EndUp = LocArr.Num();
				}
			}
		}
		RotArr.Add(UKismetMathLibrary::MakeRotFromX(Spline->GetDirectionAtTime(Fi, ESplineCoordinateSpace::World, true)));
	}
	Spline->DestroyComponent();

	if (LocArr.Num() ==1 || RotArr.Num() == 1)
	{
		return;
	}

	for (int k = LocArr.Num() - 1; k > 0; --k)
	{
		LocArr[k] = LocArr[k] - LocArr[k - 1];
	}
	LocArr[0] = FVector::ZeroVector;

	MeshShadow->SetWorldLocation(FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, 0.1f));

	GetWorldTimerManager().SetTimer(MoveTimer, [this]()
		{
			if (CurrentMoveNumber <= LocArr.Num() - 1 && CurrentMoveNumber <= RotArr.Num() - 1
				&& IsValid(Mesh) && IsValid(MeshShadow))
			{
				Mesh->AddWorldOffset(LocArr[CurrentMoveNumber]);
				Mesh->SetWorldRotation(FRotator(0.0f, RotArr[CurrentMoveNumber].Yaw, 0.0f));

				MeshShadow->AddWorldOffset(FVector(LocArr[CurrentMoveNumber].X, LocArr[CurrentMoveNumber].Y, 0.0f));

				if (CurrentMoveNumber == EndUp)
				{
					CanDown = true;
				}

				CurrentMoveNumber++;
			}
			else
			{
				LocArr.Empty(0);
				RotArr.Empty(0);
				if (MoveTimer.IsValid() && GetWorldTimerManager().TimerExists(MoveTimer))
				{
					GetWorldTimerManager().ClearTimer(MoveTimer);
				}
			}
		}, CustomDeltaTime, true);
}





//collision with a flyer or a shield
void AWild::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (IsValid(WildActor))
	{
		if (CurrentMoveNumber / (LocArr.Num() - 1) < WildActor->CurrentMoveNumber / (WildActor->LocArr.Num() - 1)
			&& !GetWorldTimerManager().IsTimerPaused(MoveTimer)
			&& MoveTimer.IsValid())
		{
			GetWorldTimerManager().PauseTimer(MoveTimer);
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		Destroy();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Internal"))
		|| OtherComp->ComponentHasTag(TEXT("InternalEnemy")))
	{
		//logic of damage
		UGameplayStatics::ApplyDamage(OtherActor, 80.0, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
		return;
	}
}


void AWild::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (IsValid(WildActor))
	{
		if (CurrentMoveNumber / (LocArr.Num() - 1) < WildActor->CurrentMoveNumber / (WildActor->LocArr.Num() - 1)
			&& GetWorldTimerManager().IsTimerPaused(MoveTimer)
			&& MoveTimer.IsValid())
		{
			GetWorldTimerManager().UnPauseTimer(MoveTimer);
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(),
			OverlappedComp->GetComponentLocation());
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
	}
}



//bugfix when flying out of the shield
void AWild::OnNoseOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), 
			OverlappedComp->GetComponentLocation());
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
	}
}




void AWild::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
}




void AWild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}





void AWild::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(CreateTimer);
	GetWorldTimerManager().ClearTimer(MoveTimer);
	GetWorldTimerManager().ClearTimer(DownTimer);
	GetWorldTimerManager().ClearTimer(DownStop);

	Super::EndPlay(EndPlayReason);
}


