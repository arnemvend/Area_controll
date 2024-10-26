// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Wild.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Structure/StructWild.h"
#include "Weapon/Gun.h"
#include "NiagaraSystem.h"



// Sets default values
AWild::AWild()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);

	MeshLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	MeshLight->SetupAttachment(Mesh);
	MeshLight->SetVisibility(false);

	MeshShadow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow"));
	MeshShadow->SetupAttachment(RootComponent);
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Weapon/Meshes/square.square'"));
	MeshShadow->SetStaticMesh(MyStaticMesh);
	MeshShadow->SetVisibility(false);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Box->SetupAttachment(Mesh);
	Box->SetRelativeLocation(FVector(3.74f, 0.0f, -2.7f));
	Box->InitBoxExtent(FVector(15.0f, 15.0f, 22.0f));

	Nose = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerNose"));
	Nose->SetupAttachment(Mesh);
	Nose->SetRelativeLocation(FVector(17.9f, 0.0f, -2.6f));
	Nose->InitBoxExtent(FVector(1.0f, 15.0f, 21.0f));

	Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	Gun->SetupAttachment(Mesh);
	Gun->SetRelativeLocation(FVector(1.36f, 0.0f, -22.0f));
	Gun->SetVisibility(false);
	//GunClass = LoadClass<AGun>(nullptr, TEXT("/Game/Weapon/BP_Gun.BP_Gun_C"));
	//static ConstructorHelpers::FClassFinder<AActor> ChildActorBP(TEXT("/Game/Weapon/BP_Gun.BP_Gun_C"));
	//Gun->SetChildActorClass(ChildActorBP.Class);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	Spline->AddSplinePointAtIndex(FVector::ZeroVector, 0, ESplineCoordinateSpace::Local);
	Spline->AddSplinePointAtIndex(FVector(100.0f, 0.0f, 0.0f), 1, ESplineCoordinateSpace::Local);
    Spline->AddSplinePointAtIndex(FVector(200.0f, 0.0f, 0.0f), 2, ESplineCoordinateSpace::Local);
	Spline->ReparamStepsPerSegment = 4;

	StartTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("StartTimeLine"));
	StartTimeLine->SetLooping(false);
	StartTimeLine->SetComponentTickInterval(0.04f);
	StartTimeLine->SetPlayRate(0.33f);

	MoveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeLine"));
	MoveTimeLine->SetLooping(false);
	MoveTimeLine->SetComponentTickInterval(0.04f);

	Type = 0;
	CurrentRotation = FRotator::ZeroRotator;

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWild::OnBoxOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AWild::OnBoxOverlapEnd);
	Nose->OnComponentEndOverlap.AddDynamic(this, &AWild::OnNoseOverlapEnd);
}




void AWild::Start()
{
	FStructWild StructWild;

	Mesh->SetStaticMesh(StructWild.SM_Main[Type]);
	MeshMaterial = StructWild.MI_Main[Type];
	Mesh->SetMaterial(0, MeshMaterial);
	DMeshMaterial = Mesh->CreateDynamicMaterialInstance(0, MeshMaterial);
	MeshLight->SetStaticMesh(StructWild.SM_Light[Type]);
	MeshLight->SetMaterial(0, StructWild.MI_Light[Type]);
	MeshShadow->SetMaterial(0, StructWild.MI_Shadow[Type]);
	NiagaraSystem = StructWild.NS_Boom[Type];
	Speed = StructWild.Speed[Type];
	High = StructWild.High[Type];
	MaxHealth = StructWild.Health[Type];
	Health = MaxHealth;
	
	TLCallbackStart.BindUFunction(this, FName("CreateFunc"));
	if (CurveFloat0)
	{
		StartTimeLine->AddInterpFloat(CurveFloat0, TLCallbackStart);
	}
	TLFinish.BindUFunction(this, FName("Continue"));
	StartTimeLine->SetTimelineFinishedFunc(TLFinish);
	StartTimeLine->ReverseFromEnd();
}

void AWild::CreateFunc(float Amount)
{
	DMeshMaterial->SetScalarParameterValue(TEXT("Amount"), Amount);
}

void AWild::Continue()
{
	MeshLight->SetVisibility(true);
	Gun->SetVisibility(true);
	AGun* GunActor = Cast<AGun>(Gun->GetChildActor());
	GunActor->Type = Type;
	GunActor->Start();
	GunActor = nullptr;
	FVector Loc;
	Loc.X = Mesh->GetComponentLocation().X;
	Loc.Y = Mesh->GetComponentLocation().Y;
	Loc.Z = 0.1f;
	MeshShadow->SetWorldLocation(Loc);
	MeshShadow->SetVisibility(true);
	Loc.X = AimCoord.X + UKismetMathLibrary::RandomFloatInRange(-150.0f, 150.0f);
	Loc.Y = AimCoord.Y + UKismetMathLibrary::RandomFloatInRange(-150.0f, 150.0f);
	Loc.Z = GetActorLocation().Z;
	Spline->SetLocationAtSplinePoint(2, Loc, ESplineCoordinateSpace::World, true);
	Loc = Spline->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::Local) * 0.5f;
	Loc.X = Loc.X + UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
	Loc.Y = Loc.Y + UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
	Spline->SetLocationAtSplinePoint(1, Loc, ESplineCoordinateSpace::Local, true);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Loc));
	MoveTimeLine->SetPlayRate((1 / Spline->GetSplineLength()) * Speed);//???
	High = High + UKismetMathLibrary::RandomFloatInRange(-40.0f, 40.0f);

	TLCallbackMove.BindUFunction(this, FName("MoveFunc"));
	TLCallbackHigh.BindUFunction(this, FName("HighFunc"));
	if (CurveFloat0 && CurveFloat1)
	{
		MoveTimeLine->AddInterpFloat(CurveFloat0, TLCallbackMove);
		MoveTimeLine->AddInterpFloat(CurveFloat1, TLCallbackHigh);
	}
	MoveTimeLine->PlayFromStart();
}



void AWild::MoveFunc(float Amount)
{
	CurrentCoord.X = Spline->GetLocationAtTime(Amount, ESplineCoordinateSpace::World, true).X;
	CurrentCoord.Y = Spline->GetLocationAtTime(Amount, ESplineCoordinateSpace::World, true).Y;
	Mesh->SetWorldLocation(CurrentCoord);
	CurrentRotation = UKismetMathLibrary::MakeRotFromX(Spline->GetDirectionAtTime(Amount, ESplineCoordinateSpace::World, true));
	Mesh->SetWorldRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	MeshShadow->SetWorldLocation(FVector(CurrentCoord.X, CurrentCoord.Y, 0.1f));
	MeshShadow->SetWorldRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
}

void AWild::HighFunc(float Amount)
{
	CurrentCoord.Z = UKismetMathLibrary::Lerp(GetActorLocation().Z, High, Amount);
}

void AWild::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (WildActor)
	{
		if (WildActor->MoveTimeLine->GetPlaybackPosition() > MoveTimeLine->GetPlaybackPosition())
		{
			MoveTimeLine->Stop();
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		Destroy();
	}
}

void AWild::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AWild* WildActor = Cast<AWild>(OtherActor);
	if (WildActor)
	{
		if (WildActor->MoveTimeLine->GetPlaybackPosition() > MoveTimeLine->GetPlaybackPosition())
		{
			MoveTimeLine->Play();
		}
	}
	WildActor = nullptr;
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		Destroy();
	}
}

void AWild::OnNoseOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		Destroy();
	}
}






// Called when the game starts or when spawned
void AWild::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AWild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWild::Destroyed()
{
	Super::Destroyed();

	const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	ABoom* Boom = GetWorld()->SpawnActor<ABoom>(Spowned, Mesh->GetComponentLocation(), SpawnRotation);
	if (Boom)
	{
		Boom->NiagaraBoomSystem = NiagaraSystem;
		Boom->NiagaraBoom->SetAsset(Boom->NiagaraBoomSystem);
		Boom->Duration = 2.0f;
		Boom->Boom();
	}
	Boom = nullptr;
}

