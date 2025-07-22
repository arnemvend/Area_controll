// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Player_HUD.generated.h"


class UGameWidget;
class UTowerWidget;
class AMainTower;
class UTowerWidget;



UCLASS()
class AREA_CONTROLL_API APlayer_HUD : public AHUD
{
	GENERATED_BODY()

public:



protected:

	virtual void BeginPlay() override;

	UPROPERTY() UGameWidget* GameWidget;
	UPROPERTY() UTowerWidget* TWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") TSubclassOf<UGameWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") TSubclassOf<UTowerWidget> TWidgetClass;

};
