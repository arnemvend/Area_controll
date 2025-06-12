// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Wild.h"
#include "Wild1.generated.h"


UCLASS()
class AREA_CONTROLL_API AWild1 : public AWild
{
	GENERATED_BODY()

public:

	AWild1();


protected:

	virtual void DestroyFunc() override;

	virtual void BeginPlay() override;
};
