// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"


class UButton;
class UTextBlock;
class UBorder;
class USlider;
class UNiagaraSystemWidget;
class UVerticalBox;
class UWidgetAnimation;
class UCheckBox;
class AMainMenu_PlayerController;
class AMainMenu_GameMode;
class FWidgetAnimationDynamicEvent;


UCLASS()
class AREA_CONTROLL_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);

protected:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY() AMainMenu_PlayerController* PController;
	UPROPERTY() AMainMenu_GameMode* GMode;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Play;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Exit;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Settings;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Y;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_N;

	UPROPERTY(meta = (BindWidget)) UButton* Button_Start;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Settings;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Low;
	UPROPERTY(meta = (BindWidget)) UButton* Button_High;
	UPROPERTY(meta = (BindWidget)) UButton* Button_ExitSettings;

	UPROPERTY(meta = (BindWidget)) UCheckBox* CheckBox_Effects;
	UPROPERTY(meta = (BindWidget)) UCheckBox* CheckBox_Music;

	UPROPERTY(meta = (BindWidget)) USlider* Slider_YesNo;

	UPROPERTY(meta = (BindWidget)) UBorder* Border_Quit;
	UPROPERTY(meta = (BindWidget)) UBorder* Border_Settings;

	UPROPERTY(meta = (BindWidget)) UVerticalBox* VerticalBox_Main;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* MainMenuAnim;
	UPROPERTY() FWidgetAnimationDynamicEvent MainMenuEvent;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* SettingsAnim;
	UPROPERTY() FWidgetAnimationDynamicEvent SettingsEvent;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* QuitAnim;
	UPROPERTY() FWidgetAnimationDynamicEvent QuitEvent;

	UPROPERTY(meta = (BindWidget)) UNiagaraSystemWidget* NSystemSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MainMenuAnimSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SettingsAnimSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float QuitAnimSpeed;

	UPROPERTY() bool ToSettings;
	UPROPERTY() bool IsYes;
	UPROPERTY() bool IsNo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float QuitTextScale;
	UPROPERTY() float FontSize;

	UPROPERTY() FSlateFontInfo FontInfo;


	UFUNCTION() void Finish_ForwardMainMenuAnim();
	UFUNCTION() void Finish_ReverseMainMenuAnim();
	UFUNCTION() void ToNewLevel();

	UFUNCTION() void Finish_ForwardSettingsAnim();
	UFUNCTION() void Finish_ReverseSettingsAnim();

	UFUNCTION() void Finish_ForwardQuitAnim();
	UFUNCTION() void Finish_ReverseQuitAnim();

	UFUNCTION() void Button_StartClick();

	UFUNCTION() void Button_ExitClick();

	UFUNCTION() void Button_SettingsClick();

	UFUNCTION() void Button_LowPress();
	UFUNCTION() void Button_HighPress();

	UFUNCTION() void Button_ExitSettingsClick();

	UFUNCTION() void Slider_YesNoProgressChanged(float Value);
	UFUNCTION() void Slider_YesNoMCaptureEnd();
	UFUNCTION() void NiagaraUpdate(float Offset);
};
