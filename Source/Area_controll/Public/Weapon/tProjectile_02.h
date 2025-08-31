// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile_0.h"
#include "tProjectile_02.generated.h"


UCLASS()
class AREA_CONTROLL_API AtProjectile_02 : public AProjectile_0
{
	GENERATED_BODY()

public:

	AtProjectile_02();

protected:

	virtual void SetterFunc() override;
};
