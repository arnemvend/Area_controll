// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"


class AAreaControll_PlayerController;
class AMainTower;
class UTextBlock;
class UButton;
class ABuildCreator;




UCLASS()
class AREA_CONTROLL_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;



protected:

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite) AAreaControll_PlayerController* P_Controller;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) AMainTower* Main; // for debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite) AMainTower* MainEnemy; // for debug
	UPROPERTY() ABuildCreator* BCreator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABuildCreator> Spowned;


	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;

	UPROPERTY() bool ShieldIsActive = false;
	UPROPERTY() bool ButtonTowerIsActive = true;
	UPROPERTY() bool ShieldIsVisible;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_FPS; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_EnemyEnergy; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_YourEnergy; // for debug

	UPROPERTY(meta = (BindWidget)) UButton* Button_Tower;
	UPROPERTY(meta = (BindWidget)) UButton* Button_AllShield;

	UPROPERTY() float DeltaTime; // for debug


	UFUNCTION() void Button_TowerPress();
	UFUNCTION() void Button_TowerReleas();
	UFUNCTION() void Button_AllShieldClick();

	UFUNCTION() void DeactivatedTowerMenu();


};
