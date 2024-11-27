// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ABoom::ABoom()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	NiagaraBoom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraBoom->SetupAttachment(RootComponent);

	Duration = 2.0f;
}



// Called when the game starts or when spawned
void ABoom::BeginPlay()
{
	Super::BeginPlay();
	
}


//LifeTime
void ABoom::Boom()
{
	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		Destroy();
	}, Duration, false);
}



void ABoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABoom::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
}

