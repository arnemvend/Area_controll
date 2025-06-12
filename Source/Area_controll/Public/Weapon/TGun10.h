// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun10.generated.h"


UCLASS()
class AREA_CONTROLL_API ATGun10 : public AGun
{
	GENERATED_BODY()

public:

	ATGun10();

protected:

	UPROPERTY() float ExtRadius;
	UPROPERTY() int ShootEnergyPrice;

	virtual void BeginPlay() override;

	virtual bool ComponentIsFar(UPrimitiveComponent* Component) override;
	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
	virtual void Fire() override;

};
