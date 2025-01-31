// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Creator/BuildCreator.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"


ABuildCreator::ABuildCreator()
{
	PrimaryActorTick.bCanEverTick = false;

	

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



	SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));

	InNumber = 0;//counter of entrances to the construction area
	ExNumber = 0;//counter of entrances to the building area
	IsReady = false;//can or can't building new tower
	NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
}



//"Trigger's functions"----------------------------------------------------------------------------------------->
void ABuildCreator::OnOverlapBegin
(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check Trigger's tag
	if (OtherComp->ComponentHasTag("External"))
	{
		ExNumber = ExNumber + 1;
		if (ExNumber < 1)
		{
			ExNumber = 1;
		}
		if (ExNumber > 0 && InNumber == 0)
		{
			IsReady = true;
			NewColor = FLinearColor::Green;
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
	}

	if (OtherComp->ComponentHasTag("Internal") || OtherComp->ComponentHasTag("InternalEnemy"))
	{
		InNumber = InNumber + 1;
		if (InNumber < 1)
		{
			InNumber = 1;
		}
		IsReady = false;
		NewColor = FLinearColor::Red;
		DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
		DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
	}
}

void ABuildCreator::OnOverlapEnd
(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check Trigger's tag
	if (OtherComp->ComponentHasTag("External"))
	{
		ExNumber = ExNumber - 1;
		if (ExNumber < 0)
		{
			ExNumber = 0;
		}
		if (ExNumber == 0 || InNumber > 0)
		{
			IsReady = false;
			NewColor = FLinearColor::Red;
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
	}

	if (OtherComp->ComponentHasTag("Internal") || OtherComp->ComponentHasTag("InternalEnemy"))
	{
		InNumber = InNumber - 1;
		if (InNumber < 0)
		{
			InNumber = 0;
		}
		if (InNumber == 0 && ExNumber > 0)
		{
			IsReady = true;
			NewColor = FLinearColor::Green;
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





//"BeginPlay. It works when game starts"---------------------------------------------------------------------->
void ABuildCreator::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = StaticMesh->CreateDynamicMaterialInstance(0, CreatorMaterial);
	DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
	DynamicMaterial0 = StaticMesh0->CreateDynamicMaterialInstance(0, CreatorMaterial0);
	DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor); //set color
	//checking the movement
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			if (GetActorLocation().Z < 0.0f)
			{
				Destroy();
			}
		}, 0.1f, false);
}

void ABuildCreator::Destroyed()
{
	Super::Destroyed();

	ABuildCreator::SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));
	GetWorldTimerManager().ClearTimer(Timer0);
}


//"Tick. It works every tick, if EverTick = true"------------------------------------------------------------->
void ABuildCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


