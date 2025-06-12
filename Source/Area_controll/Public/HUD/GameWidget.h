// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/GameWidgetInterface.h"
#include "GameWidget.generated.h"


class AAreaControll_PlayerController;
class AAreaControll_GameMode;
class UTextBlock;
class UButton;
class ABuildCreator;




UCLASS()
class AREA_CONTROLL_API UGameWidget : public UUserWidget, public IGameWidgetInterface
{
	GENERATED_BODY()

public:

	virtual FOnDeTouchDelegate& GetOnDeTouchDelegate() override;
	virtual FOnShieldDelegate& GetOnShieldDelegate() override;


protected:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



	UPROPERTY() AAreaControll_PlayerController* P_Controller;
	UPROPERTY() AAreaControll_GameMode* GMode;
	UPROPERTY() ABuildCreator* BCreator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABuildCreator> Spowned;


	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;


	UPROPERTY() bool ShieldIsActive = false;
	UPROPERTY() bool ButtonTowerIsActive = true;
	UPROPERTY() bool ShieldIsVisible;
	UPROPERTY() int TowerPrice;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_FPS; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_EnemyEnergy; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_YourEnergy; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_ForTests; // for debug
	//UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Cn_EnergyPrice;

	UPROPERTY(meta = (BindWidget)) UButton* Button_Tower;
	UPROPERTY(meta = (BindWidget)) UButton* Button_AllShield;

	UPROPERTY() float DeltaTime; // for debug


	UPROPERTY() FOnDeTouchDelegate OnDeTouchDelegate;
	UPROPERTY() FOnShieldDelegate OnShieldDelegate;


	UFUNCTION() void Button_TowerPress();
	UFUNCTION() void Button_TowerReleas();
	UFUNCTION() void Button_AllShieldClick();

	UFUNCTION() void DeactivatedTowerMenu();

	
};
