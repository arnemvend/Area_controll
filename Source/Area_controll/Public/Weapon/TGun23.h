// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TGun2.h"
#include "TGun23.generated.h"




UCLASS()
class AREA_CONTROLL_API ATGun23 : public ATGun2
{
	GENERATED_BODY()

public:

	ATGun23();

protected:

	virtual void BeginPlay() override;

	/*UPROPERTY() float ExtRadius;
	UPROPERTY() int ShootEnergyPrice;

	virtual bool ComponentIsFar(UPrimitiveComponent* Component) override;
	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;*/
	virtual void Fire() override;
};
