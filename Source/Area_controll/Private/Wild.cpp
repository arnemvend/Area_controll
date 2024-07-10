// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Wild.h"

// Sets default values
AWild::AWild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

