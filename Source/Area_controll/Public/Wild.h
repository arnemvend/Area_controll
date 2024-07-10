// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wild.generated.h"

UCLASS()
class AREA_CONTROLL_API AWild : public AActor
{
	GENERATED_BODY()
	
public:	

	AWild();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;




};
