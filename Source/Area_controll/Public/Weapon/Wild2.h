// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Wild.h"
#include "Wild2.generated.h"


UCLASS()
class AREA_CONTROLL_API AWild2 : public AWild
{
	GENERATED_BODY()

public:

	AWild2();

protected:

	virtual void Destroyed() override;

	virtual void BeginPlay() override;
};
