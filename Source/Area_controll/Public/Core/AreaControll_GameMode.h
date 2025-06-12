// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AreaControll_GameMode.generated.h"


class ATower;



UCLASS()
class AREA_CONTROLL_API AAreaControll_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAreaControll_GameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<ATower*> PlayerTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<ATower*> EnemyTowers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<ATower*> DisabledTowers;

	UPROPERTY() int PlayerEnergy;
	UPROPERTY() int EnemyEnergy;
	UPROPERTY() int PlayerMaxEnergy;
	UPROPERTY() int EnemyMaxEnergy;

};
