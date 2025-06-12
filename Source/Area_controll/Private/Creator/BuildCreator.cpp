// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Creator/BuildCreator.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"


ABuildCreator::ABuildCreator()
{
	PrimaryActorTick.bCanEverTick = false;

	

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));



	//"Set parameters of Static Mesh"--------------------------------------------------------------------------->
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/EnergyTower.EnergyTower'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statick Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetStaticMesh(MyStaticMesh);
	CreatorMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_BCreator.MI_BCreator'"));
	StaticMesh->SetMaterial(0, CreatorMaterial);

	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/pipe.pipe'"));
	StaticMesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statick Mesh 0"));
	StaticMesh0->SetupAttachment(RootComponent);
	StaticMesh0->SetStaticMesh(MyStaticMesh);
	CreatorMaterial0 = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Pipe.MI_Pipe'"));
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
		ExNumber = FMath::Max(1, ExNumber + 1);
		if (ExNumber > 0 && InNumber == 0 && CanBuild)
		{
			IsReady = true;
			NewColor = FLinearColor::Green;
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
		ExternalComponents.AddUnique(OtherComp);
		return;
	}

	if (OtherComp->ComponentHasTag("Internal")
		|| OtherComp->ComponentHasTag("InternalEnemy")
		|| OtherComp->ComponentHasTag("NeutralInternal")
		|| OtherComp->ComponentHasTag("InternalWild"))
	{
		InNumber = FMath::Max(1, InNumber + 1);
		IsReady = false;
		NewColor = FLinearColor::Red;
		DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
		DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		return;
	}

	if (OtherComp->ComponentHasTag("NeutralExternal"))
	{
		NeutralComponents.AddUnique(OtherComp);
	}
}

void ABuildCreator::OnOverlapEnd
(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check Trigger's tag
	if (OtherComp->ComponentHasTag("External"))
	{
		ExNumber = FMath::Max(0, ExNumber - 1);
		if (ExNumber == 0 || InNumber > 0)
		{
			IsReady = false;
			NewColor = FLinearColor::Red;
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
		ExternalComponents.RemoveSwap(OtherComp);
		return;
	}

	if (OtherComp->ComponentHasTag("Internal")
		|| OtherComp->ComponentHasTag("InternalEnemy")
		|| OtherComp->ComponentHasTag("NeutralInternal")
		|| OtherComp->ComponentHasTag("InternalWild"))
	{
		InNumber = FMath::Max(0, InNumber - 1);
		if (InNumber == 0 && ExNumber > 0 && CanBuild)
		{
			IsReady = true;
			NewColor = FLinearColor::Green;
			DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
			DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
		}
		return;
	}

	if (OtherComp->ComponentHasTag("NeutralExternal"))
	{
		NeutralComponents.RemoveSwap(OtherComp);
	}
}



//"Control location of this Actor"---------------------------------------------------------------------------->
void ABuildCreator::MovingFunc(FVector2D Loc)
{
	if (n < 2)
	{
		n++;
	}
	else
	{
		SetActorLocation(FVector(Loc.X, Loc.Y, 0.1f));
	}
}






void ABuildCreator::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());

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

	//checking neutral triggers
	GetWorldTimerManager().SetTimer(Timer1, [this]()
		{
			if (IsValid(GMode) && IsValid(GInstance))
			{
				if (GMode->PlayerEnergy < GInstance->Cn_EnergyPrice)
				{
					CanBuild = false;
				}
				else
				{
					CanBuild = true;
					if (ExNumber > 0 && InNumber == 0 && CanBuild)
					{
						IsReady = true;
						NewColor = FLinearColor::Green;
						DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
						DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
					}
				}
			}

			if (ExternalComponents.Num() > 0)
			{
				for (int i = ExternalComponents.Num() - 1; i >= 0; --i)
				{
					if (!IsValid(ExternalComponents[i]))
					{
						ExternalComponents.RemoveAtSwap(i);
						continue;
					}

					if (IsValid(ExternalComponents[i]) && ExternalComponents[i]->ComponentHasTag("NeutralExternal"))
					{
						ExNumber = FMath::Max(0, ExNumber - 1);
						if (ExNumber == 0 || InNumber > 0)
						{
							IsReady = false;
							NewColor = FLinearColor::Red;
							DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor); //set color
							DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
						}
						NeutralComponents.AddUnique(ExternalComponents[i]);
						ExternalComponents.RemoveAtSwap(i);
					}
				}
			}
			if (NeutralComponents.Num() > 0)
			{
				for (int i = NeutralComponents.Num() - 1; i >= 0; --i)
				{
					if (!IsValid(NeutralComponents[i]))
					{
						NeutralComponents.RemoveAtSwap(i);
						continue;
					}

					if (IsValid(NeutralComponents[i]) && NeutralComponents[i]->ComponentHasTag("External"))
					{
						ExNumber = FMath::Max(ExNumber + 1, 1);
						if (ExNumber > 0 && InNumber == 0 && CanBuild)
						{
							IsReady = true;
							NewColor = FLinearColor::Green;

							DynamicMaterial->SetVectorParameterValue(TEXT("CreatorColor"), NewColor);
							DynamicMaterial0->SetVectorParameterValue(TEXT("Color"), NewColor);
						}
						ExternalComponents.AddUnique(NeutralComponents[i]);
						NeutralComponents.RemoveAtSwap(i);
					}
				}
			}
		}, 0.2f, true, 0.0f);
}





void ABuildCreator::Destroyed()
{
	

	SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));
	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);

	Super::Destroyed();
}



void ABuildCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


