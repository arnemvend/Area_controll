// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerWidget.generated.h"



class UButton;
class ATower;


UCLASS()
class AREA_CONTROLL_API UTowerWidget : public UUserWidget
{
	GENERATED_BODY()



public:

	UPROPERTY() ATower* MyTower;

	UFUNCTION() void StartEvent();

	UFUNCTION() void Reset();


protected:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget)) UButton* Button_shield;
	UPROPERTY(meta = (BindWidget)) UButton* Button_destroy;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Repeater;

	UFUNCTION() void Button_RepeaterClick();
	UFUNCTION() void Button_destroyClick();
	UFUNCTION() void Button_shieldClick();









};
