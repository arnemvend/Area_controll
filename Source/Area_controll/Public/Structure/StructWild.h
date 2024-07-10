// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructWild.generated.h"

USTRUCT(BlueprintType)

struct AREA_CONTROLL_API FStructWild
{
	GENERATED_BODY()

public:

	FStructWild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Damage;
};
