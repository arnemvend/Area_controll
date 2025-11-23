// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/GameWidgetInterface.h"
#include "GameWidget.generated.h"


class AAreaControll_PlayerController;
class AAreaControll_GameMode;
class ABuildCreator;
class UBackgroundBlur;
class UTextBlock;
class UButton;
class UImage;
class URadialSlider;
class UOverlay;
class UMaterialInstanceDynamic;
struct FButtonStyle;




UCLASS()
class AREA_CONTROLL_API UGameWidget : public UUserWidget, public IGameWidgetInterface
{
	GENERATED_BODY()

public:

	UGameWidget(const FObjectInitializer& ObjectInitializer);

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


	UPROPERTY() UMaterialInstanceDynamic* EnBarDMaterial;
	UPROPERTY() UMaterialInstanceDynamic* PauseSliderDMaterial;


	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY() FTimerHandle Timer2;


	UPROPERTY() bool ShieldIsActive;
	UPROPERTY() bool ButtonTowerIsActive;
	UPROPERTY() bool ShieldIsVisible;
	UPROPERTY() bool IsResume;
	UPROPERTY() bool IsRestart;
	UPROPERTY() bool IsRetreat;
	UPROPERTY() bool CanChange;
	UPROPERTY() bool IsFirstCheck;

	UPROPERTY() int TowerPrice;
	UPROPERTY() int EnergyLoss;
	UPROPERTY() int CurrentEnergy;
	
	UPROPERTY() float DeltaTime; // for debug
	UPROPERTY() float FontSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float PauseTextScale;
	

	UPROPERTY() FButtonStyle CurrentStile;
	UPROPERTY() FButtonStyle OffStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FButtonStyle OnStyle;
	UPROPERTY() FSlateFontInfo FontInfo;


	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_FPS; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_EnemyEnergy; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_YourEnergy; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_ForTests; // for debug
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_Resume;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_restart;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_Retreat;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_TowerPrice;

	UPROPERTY(meta = (BindWidget)) UButton* Button_Tower;
	UPROPERTY(meta = (BindWidget)) UButton* Button_AllShield;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Pause;

	UPROPERTY(meta = (BindWidget)) UImage* EnergyBarImage;
	UPROPERTY(meta = (BindWidget)) UImage* Image_Pause;

	UPROPERTY(meta = (BindWidget)) UBackgroundBlur* Blur_Pause;

	UPROPERTY(meta = (BindWidget)) URadialSlider* RadialSlider_Pause;

	UPROPERTY(meta = (BindWidget)) UOverlay* Overlay_Pause;



	UPROPERTY() FOnDeTouchDelegate OnDeTouchDelegate;
	UPROPERTY() FOnShieldDelegate OnShieldDelegate;



	UFUNCTION() void Button_TowerPress();
	UFUNCTION() void Button_TowerReleas();
	UFUNCTION() void Button_AllShieldClick();
	UFUNCTION() void Button_PauseClick();
	UFUNCTION() void RadialSliderChanged(float Value);
	UFUNCTION() void RadialSliderCaptureEnd();
	UFUNCTION() void RadialSliderCaptureBegin();
	UFUNCTION() void DeactivatedTowerMenu();
	UFUNCTION() void UpdateBar();
	UFUNCTION() void SetDMaterialFunc(UImage*& Image, UMaterialInstanceDynamic*& DMaterial);
	
};
