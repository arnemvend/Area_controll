// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "Gun2.generated.h"





UCLASS()
class AREA_CONTROLL_API AGun2 : public AGun
{
	GENERATED_BODY()

public:

	AGun2();

	virtual void TimerElapsed() override;

protected:

	virtual void Rotate(float Amount) override;

	virtual void BeginPlay() override;

	virtual void FreeRotate(float Amount) override;
	virtual void Fire() override;
	virtual void Tracking() override;
};
