// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/PreloadActor.h"



// Sets default values
APreloadActor::APreloadActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	
}




// Called when the game starts or when spawned
void APreloadActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APreloadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



