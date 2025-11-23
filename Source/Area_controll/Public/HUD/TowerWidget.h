// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerWidget.generated.h"



class UButton;
class UVerticalBox;
class UHorizontalBox;
class UTextBlock;
class UImage;
class UCheckBox;
class ATower;
class AAreaRadius;
class UAreaControll_GameInstance;
class AAreaControll_GameMode;



UCLASS()
class AREA_CONTROLL_API UTowerWidget : public UUserWidget
{
	GENERATED_BODY()



public:

	UPROPERTY() ATower* MyTower;
	UPROPERTY() AAreaRadius* AreaRadius;

	virtual void NativeConstruct() override;

	UFUNCTION() void StartEvent();

	UFUNCTION() void Reset();

protected:

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;

	UPROPERTY() UAreaControll_GameInstance* GInstance;
	UPROPERTY() AAreaControll_GameMode* GMode;

	//UPROPERTY(meta = (BindWidget)) UBorder* Border_Guns;
	//UPROPERTY(meta = (BindWidget)) UBorder* Border_LowGuns;
	//UPROPERTY(meta = (BindWidget)) UBorder* Border_MidGuns;
	//UPROPERTY(meta = (BindWidget)) UBorder* Border_UpGuns;

	UPROPERTY(meta = (BindWidget)) UVerticalBox* VerticalBox_Guns;
	UPROPERTY(meta = (BindWidget)) UVerticalBox* VerticalBox_Mod;

	UPROPERTY(meta = (BindWidget)) UHorizontalBox* HorizontalBox_UpGuns;
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* HorizontalBox_MidGuns;
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* HorizontalBox_LowGuns;


	UPROPERTY(meta = (BindWidget)) UImage* Image_Up;
	UPROPERTY(meta = (BindWidget)) UImage* Image_Mid;
	UPROPERTY(meta = (BindWidget)) UImage* Image_Low;


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
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun02;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun03;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Delete1;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun10;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun11;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun12;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun13;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Delete2;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun20;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun21;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun22;
	UPROPERTY(meta = (BindWidget)) UButton* Button_gun23;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Aim;
	UPROPERTY(meta = (BindWidget)) UButton* Button_modify;

	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun00;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun01;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun02;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun03;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun10;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun11;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun12;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun13;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun20;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun21;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun22;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_gun23;

	UPROPERTY(meta = (BindWidget)) UCheckBox* CheckBox_Auto;
	


	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;


	UPROPERTY() FButtonStyle ButtonStyle;
	UPROPERTY() FButtonStyle ButtonPriceStyle;
	UPROPERTY() UButton* ActiveButton;
	UPROPERTY() UButton* ActiveGunButton;
	UPROPERTY() TArray<UButton*> UnActiveButtons;


	UPROPERTY() int AdressUpGun;
	UPROPERTY() int AdressMidGun;
	UPROPERTY() int AdressLowGun;

	UPROPERTY() bool IsFirst;
	

	UFUNCTION() void MoveStyle();
	UFUNCTION() void MovePriceStyle(UButton*& Button);
	UFUNCTION() void CloseGuns();
	UFUNCTION() bool PriceUpdate(bool IsGood, UButton*& Button);
	UFUNCTION() void ButtonGunFunc(UButton*& Button, UChildActorComponent*& Component, 
		int Type, int Number, UHorizontalBox*& Box, UImage*& Image, FTimerHandle& Timer);
	UFUNCTION() void DeleteGunFunc(UButton*& Button, UChildActorComponent*& Component, 
		int Type, UHorizontalBox*& Box);
	UFUNCTION() void Button_RepeaterClick();
	UFUNCTION() void Button_destroyClick();
	UFUNCTION() void Button_shieldClick();
	UFUNCTION() void Button_modifyClick();
	UFUNCTION() void Button_gunsClick();
	UFUNCTION() void Button_UpGunClick();
	UFUNCTION() void Button_MidGunClick();
	UFUNCTION() void Button_LowGunClick();
	UFUNCTION() void Button_gun00Click();
	UFUNCTION() void Button_gun01Click();
	UFUNCTION() void Button_gun02Click();
	UFUNCTION() void Button_gun03Click();
	UFUNCTION() void Button_gun10Click();
	UFUNCTION() void Button_gun11Click();
	UFUNCTION() void Button_gun12Click();
	UFUNCTION() void Button_gun13Click();
	UFUNCTION() void Button_gun20Click();
	UFUNCTION() void Button_gun21Click();
	UFUNCTION() void Button_gun22Click();
	UFUNCTION() void Button_gun23Click();
	UFUNCTION() void Button_Delete0Click();
	UFUNCTION() void Button_Delete1Click();
	UFUNCTION() void Button_Delete2Click();
	UFUNCTION() void CheckAutoOnChecked(bool bCheck);
	UFUNCTION() void Button_AimClick();






};
