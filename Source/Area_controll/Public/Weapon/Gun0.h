// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "Gun0.generated.h"



UCLASS()
class AREA_CONTROLL_API AGun0 : public AGun
{
	GENERATED_BODY()

public:

	AGun0();



protected:

	virtual void Rotate(float Amount) override;

	virtual void BeginPlay() override;

};
