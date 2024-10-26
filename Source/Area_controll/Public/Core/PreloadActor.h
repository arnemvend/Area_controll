// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreloadActor.generated.h"



UCLASS()
class AREA_CONTROLL_API APreloadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreloadActor();

	virtual void Tick(float DeltaTime) override;



protected:



	virtual void BeginPlay() override;


};
