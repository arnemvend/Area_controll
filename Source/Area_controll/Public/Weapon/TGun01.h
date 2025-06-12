// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun01.generated.h"





UCLASS()
class AREA_CONTROLL_API ATGun01 : public AGun
{
	GENERATED_BODY()

public:

	ATGun01();

protected:

	UPROPERTY() int ShootEnergyPrice;

	virtual void BeginPlay() override;

	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
	virtual void Fire() override;

};
