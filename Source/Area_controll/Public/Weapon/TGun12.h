// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun12.generated.h"





UCLASS()
class AREA_CONTROLL_API ATGun12 : public AGun
{
	GENERATED_BODY()

public:

	ATGun12();

protected:

	UPROPERTY() int ShootEnergyPrice;

	virtual void BeginPlay() override;

	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
	virtual void Fire() override;
};
