// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Creator/BuildCreator.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"


ABuildCreator::ABuildCreator()
{
	PrimaryActorTick.bCanEverTick = false;

	ABuildCreator::SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));

	InNumber = 0;
	ExNumber = 0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));



	//"Set parameters of Static Mesh"--------------------------------------------------------------------------->
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/EnergyTower.EnergyTower'"));//set mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statick Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetStaticMesh(MyStaticMesh);
	CreatorMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_BCreator.MI_BCreator'"));//set material
	StaticMesh->SetMaterial(0, CreatorMaterial);

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/pipe.pipe'"));//set mesh
	StaticMesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statick Mesh 0"));
	StaticMesh0->SetupAttachment(RootComponent);
	StaticMesh0->SetStaticMesh(MyStaticMesh);
	CreatorMaterial0 = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Pipe.MI_Pipe'"));//set material
	StaticMesh0->SetMaterial(0, CreatorMaterial0);


	
	

	//"Set parameters of Trigger"------------------------------------------------------------------------------->
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	TriggerCapsule->SetCollisionProfileName(TEXT("OverlapAll"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABuildCreator::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABuildCreator::OnOverlapEnd);

	IsReady = false;//can or can't building new tower
	NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
	i = 0;
}



//"Trigger's functions"----------------------------------------------------------------------------------------->
void ABuildCreator::OnOverlapBegin
(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check Trigger's tag
	if (OtherActor && OtherComp->ComponentHasTag("External") && CreatorMaterial)
	{
		ExNumber = ExNumber + 1;
		if (ExNumber > 0 && InNumber == 0)
		{
			IsReady = true;
			NewColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
	}

	if (OtherActor && (OtherComp->ComponentHasTag("Internal") || OtherComp->ComponentHasTag("InternalEnemy")) && CreatorMaterial)
	{
		InNumber = InNumber + 1;
		IsReady = false;
		NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
		DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
		DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
	}
}

void ABuildCreator::OnOverlapEnd
(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check Trigger's tag
	if (OtherActor && OtherComp->ComponentHasTag("External") && CreatorMaterial)
	{
		ExNumber = ExNumber - 1;
		if (ExNumber == 0 || InNumber > 0)
		{
			IsReady = false;
			NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
	}

	if (OtherActor && (OtherComp->ComponentHasTag("Internal") || OtherComp->ComponentHasTag("InternalEnemy")) && CreatorMaterial)
	{
		InNumber = InNumber - 1;
		if (InNumber == 0 && ExNumber > 0)
		{
			IsReady = true;
			NewColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
	}
}



//"Control location of this Actor"---------------------------------------------------------------------------->
void ABuildCreator::MovingFunc(FVector2D Loc)
{
	if (i < 2)
	{
		i++;
	}
	else
	{
		SetActorLocation(FVector(Loc.X, Loc.Y, 0.1f));
	}
}



//"Destroy Actor process"------------------------------------------------------------------------------------->
void ABuildCreator::DestroyerFunc()
{
	ABuildCreator::SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));
	GetWorldTimerManager().ClearTimer(Timer0);
	Destroy();
}



//"BeginPlay. It works when game starts"---------------------------------------------------------------------->
void ABuildCreator::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = StaticMesh->CreateDynamicMaterialInstance(0, CreatorMaterial);
	DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
	DynamicMaterial0 = StaticMesh0->CreateDynamicMaterialInstance(0, CreatorMaterial0);
	DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor); //set color

	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			if (GetActorLocation().Z < 0.0f)
			{
				DestroyerFunc();
			}
		}, 0.1f, false);
}



//"Tick. It works every tick, if EverTick = true"------------------------------------------------------------->
void ABuildCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


