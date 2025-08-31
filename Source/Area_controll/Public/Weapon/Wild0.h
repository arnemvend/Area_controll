// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Wild.h"
#include "Wild0.generated.h"


UCLASS()
class AREA_CONTROLL_API AWild0 : public AWild
{
	GENERATED_BODY()


public:

	AWild0();


protected:

	virtual void Destroyed() override;

	virtual void BeginPlay() override;
};
