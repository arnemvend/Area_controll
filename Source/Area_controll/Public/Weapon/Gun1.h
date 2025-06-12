// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "Gun1.generated.h"




UCLASS()
class AREA_CONTROLL_API AGun1 : public AGun
{
	GENERATED_BODY()


public:

	AGun1();



protected:

	virtual void Rotate(float Amount) override;

	virtual void BeginPlay() override;

};
