// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ABoom::ABoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	NiagaraBoom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraBoom->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABoom::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoom::Boom()
{
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		Destroy();
	}, Duration, false);
}

// Called every frame
void ABoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoom::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

