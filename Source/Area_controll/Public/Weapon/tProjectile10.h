// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/tProjectile00.h"
#include "tProjectile10.generated.h"


UCLASS()
class AREA_CONTROLL_API AtProjectile10 : public AtProjectile00
{
	GENERATED_BODY()

public:

	AtProjectile10();

protected:

	virtual void SetDamage() override;

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
