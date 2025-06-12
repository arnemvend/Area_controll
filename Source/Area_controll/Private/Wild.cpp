// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Wild.h"
#include "Boom.h"
#include "NiagaraComponent.h"
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

	StartTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("StartTimeLine"));
	StartTimeLine->SetLooping(false);
	StartTimeLine->SetComponentTickInterval(0.04f);
	StartTimeLine->SetPlayRate(0.33f);

	MoveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeLine"));
	MoveTimeLine->SetLooping(false);
	MoveTimeLine->SetComponentTickInterval(0.04f);

	CurrentRotation = FRotator::ZeroRotator;
	EndScaleShMesh = 0.15f;

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWild::OnBoxOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AWild::OnBoxOverlapEnd);
	Nose->OnComponentEndOverlap.AddDynamic(this, &AWild::OnNoseOverlapEnd);

	OnTakeAnyDamage.AddDynamic(this, &AWild::OnTakeDamage);

	CanDamage = false;
}



void AWild::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Health > 0.0f && CanDamage)
	{
		Health = Health - Damage;
		if (Health <= 0.0f)
		{
			DestroyFunc();
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
	
	TLCallbackStart.BindUFunction(this, FName("CreateFunc"));
	if (CurveFloat0)
	{
		StartTimeLine->AddInterpFloat(CurveFloat0, TLCallbackStart);
		TLFinish.BindUFunction(this, FName("Continue"));
		StartTimeLine->SetTimelineFinishedFunc(TLFinish);
		StartTimeLine->ReverseFromEnd();
	}
}



//spown material FX
void AWild::CreateFunc(float Amount)
{
	DMeshMaterial->SetScalarParameterValue(TEXT("Amount"), Amount);
}



//setting the trajectory in spline local space
void AWild::Continue()
{
	StartTimeLine->DestroyComponent();
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
	
	//set speed and high of flight
	MoveTimeLine->SetPlayRate((1 / Spline->GetSplineLength()) * Speed);
	High = High + UKismetMathLibrary::RandomFloatInRange(-40.0f, 40.0f);
	
	TLCallbackMove.BindUFunction(this, FName("MoveFunc"));
	TLCallbackHigh.BindUFunction(this, FName("HighFunc"));
	TLFinish.BindUFunction(this, FName("FinishFunc"));
	if (CurveFloat0 && CurveFloat1 && IsValid(Spline))
	{
		MoveTimeLine->AddInterpFloat(CurveFloat0, TLCallbackMove);
		MoveTimeLine->AddInterpFloat(CurveFloat1, TLCallbackHigh);
		MoveTimeLine->SetTimelineFinishedFunc(TLFinish);
		MoveTimeLine->PlayFromStart();
	} 
}

//flight in XY axes
void AWild::MoveFunc(float Amount)
{
	//mesh movement
	CurrentCoord.X = Spline->GetLocationAtTime(Amount, ESplineCoordinateSpace::World, true).X;
	CurrentCoord.Y = Spline->GetLocationAtTime(Amount, ESplineCoordinateSpace::World, true).Y;
	Mesh->SetWorldLocation(CurrentCoord);
	CurrentRotation = UKismetMathLibrary::MakeRotFromX
	(Spline->GetDirectionAtTime(Amount, ESplineCoordinateSpace::World, true));
	Mesh->SetWorldRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	//shadow movement
	MeshShadow->SetWorldLocation(FVector(CurrentCoord.X, CurrentCoord.Y, 0.1f));
}



//flight in Z axis
void AWild::HighFunc(float Amount)
{
	CurrentCoord.Z = UKismetMathLibrary::Lerp(GetActorLocation().Z, High, Amount);
	CurrentScaleShMesh.X = UKismetMathLibrary::Lerp(CurrentScaleShMesh.X, EndScaleShMesh, Amount);
	CurrentScaleShMesh.Y = UKismetMathLibrary::Lerp(CurrentScaleShMesh.Y, EndScaleShMesh, Amount);
	MeshShadow->SetRelativeScale3D(CurrentScaleShMesh);
	if (Amount == 1.0)
	{
		TLCallbackHigh.Unbind();
	}
}

void AWild::FinishFunc()
{
	MoveTimeLine->Stop();
	Spline->DestroyComponent();
}




//collision with a flyer or a shield
void AWild::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (IsValid(WildActor))
	{
		if (IsValid(WildActor->MoveTimeLine) && IsValid(MoveTimeLine))
		{
			if (WildActor->MoveTimeLine->GetPlaybackPosition() > MoveTimeLine->GetPlaybackPosition())
			{
				MoveTimeLine->Stop();
			}
		}
		if (IsValid(MoveTimeLine) && MoveTimeLine->IsPlaying())
		{
			MoveTimeLine->Stop();
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		DestroyFunc();
	}
	if (OtherComp->ComponentHasTag(TEXT("Internal"))
		|| OtherComp->ComponentHasTag(TEXT("InternalEnemy")))
	{
		//logic of damage
		UGameplayStatics::ApplyDamage(OtherActor, 20.0, GetInstigatorController(), this, UDamageType::StaticClass());
		
	}
}


void AWild::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (IsValid(WildActor))
	{
		if (IsValid(WildActor->MoveTimeLine) && IsValid(MoveTimeLine))
		{
			if (WildActor->MoveTimeLine->GetPlaybackPosition() > MoveTimeLine->GetPlaybackPosition() && IsValid(Spline))
			{
				MoveTimeLine->Play();
			}
		}
		if (IsValid(MoveTimeLine) && MoveTimeLine->IsPlaying() == false && IsValid(Spline))
		{
			MoveTimeLine->Play();
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(),
			OverlappedComp->GetComponentLocation());
		BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		DestroyFunc();
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
		BoomActor->CreateBoomFunc(OverlappedComp->GetComponentLocation(), SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		DestroyFunc();
	}
}




void AWild::BeginPlay()
{
	Super::BeginPlay();

	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));

	CurrentScaleShMesh = MeshShadow->GetRelativeScale3D();
}




void AWild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AWild::DestroyFunc()
{
	Destroy();
}


