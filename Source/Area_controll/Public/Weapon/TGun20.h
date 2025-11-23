// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TGun2.h"
#include "TGun20.generated.h"



UCLASS()
class AREA_CONTROLL_API ATGun20 : public ATGun2
{
	GENERATED_BODY()
	
public:	
	
	ATGun20();

protected:
	
	virtual void BeginPlay() override;

	/*UPROPERTY() float ExtRadius;
	UPROPERTY() int ShootEnergyPrice;

	virtual bool ComponentIsFar(UPrimitiveComponent* Component) override;
	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;*/
	virtual void Fire() override;
};
