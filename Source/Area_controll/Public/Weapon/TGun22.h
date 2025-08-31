// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun22.generated.h"





UCLASS()
class AREA_CONTROLL_API ATGun22 : public AGun
{
	GENERATED_BODY()

public:

	ATGun22();

protected:

	virtual void BeginPlay() override;

	UPROPERTY() float ExtRadius;
	UPROPERTY() int ShootEnergyPrice;

	virtual bool ComponentIsFar(UPrimitiveComponent* Component) override;
	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
	virtual void Fire() override;
};
