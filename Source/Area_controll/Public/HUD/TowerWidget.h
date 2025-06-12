// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerWidget.generated.h"



class UButton;
class ATower;
class UAreaControll_GameInstance;
class AAreaControll_GameMode;


UCLASS()
class AREA_CONTROLL_API UTowerWidget : public UUserWidget
{
	GENERATED_BODY()



public:

	UPROPERTY() ATower* MyTower;

	virtual void NativeConstruct() override;

	UFUNCTION() void StartEvent();

	UFUNCTION() void Reset();

protected:

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;

	UPROPERTY() UAreaControll_GameInstance* GInstance;
	UPROPERTY() AAreaControll_GameMode* GMode;

	UPROPERTY(meta = (BindWidget)) UButton* Button_shield;
	UPROPERTY(meta = (BindWidget)) UButton* Button_destroy;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Repeater;
	UPROPERTY(meta = (BindWidget)) UButton* Button_guns;
	UPROPERTY(meta = (BindWidget)) UButton* Button_UpGun;
	UPROPERTY(meta = (BindWidget)) UButton* Button_MidGun;
	UPROPERTY(meta = (BindWidget)) UButton* Button_LowGun;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Delete0;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun00;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun01;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Delete1;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun10;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun11;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Delete2;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun20;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun21;

	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;

	UFUNCTION() void Button_RepeaterClick();
	UFUNCTION() void Button_destroyClick();
	UFUNCTION() void Button_shieldClick();
	UFUNCTION() void Button_gunsClick();
	UFUNCTION() void Button_UpGunClick();
	UFUNCTION() void Button_MidGunClick();
	UFUNCTION() void Button_LowGunClick();
	UFUNCTION() void Button_gun00Click();
	UFUNCTION() void Button_gun01Click();
	UFUNCTION() void Button_gun10Click();
	UFUNCTION() void Button_gun11Click();
	UFUNCTION() void Button_gun20Click();
	UFUNCTION() void Button_gun21Click();
	UFUNCTION() void Button_Delete0Click();
	UFUNCTION() void Button_Delete1Click();
	UFUNCTION() void Button_Delete2Click();







};
