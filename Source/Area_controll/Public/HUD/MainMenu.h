// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"


class UButton;
class UTextBlock;
class UBorder;
class AMainMenu_PlayerController;


UCLASS()
class AREA_CONTROLL_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

protected:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY() AMainMenu_PlayerController* PController;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Play;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Exit;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Settings;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Y;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_N;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Low;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Medium;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_High;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_ExitSettings;

	UPROPERTY(meta = (BindWidget)) UButton* Button_Start;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Settings;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Yes;
	UPROPERTY(meta = (BindWidget)) UButton* Button_No;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Low;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Medium;
	UPROPERTY(meta = (BindWidget)) UButton* Button_High;
	UPROPERTY(meta = (BindWidget)) UButton* Button_ExitSettings;

	UPROPERTY(meta = (BindWidget)) UBorder* Border_Quit;
	UPROPERTY(meta = (BindWidget)) UBorder* Border_Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TextPlayOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TextSettingsOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TextExitOffset;

	UFUNCTION() void Button_StartPress();
	UFUNCTION() void Button_StartRelease();
	UFUNCTION() void Button_StartClick();

	UFUNCTION() void Button_ExitPress();
	UFUNCTION() void Button_ExitRelease();
	UFUNCTION() void Button_ExitClick();

	UFUNCTION() void Button_SettingsPress();
	UFUNCTION() void Button_SettingsRelease();
	UFUNCTION() void Button_SettingsClick();

	UFUNCTION() void Button_LowPress();
	UFUNCTION() void Button_MediumPress();
	UFUNCTION() void Button_HighPress();
	UFUNCTION() void VideoSettingsReClick(UButton* Button1, UButton* Button2, UTextBlock* Text1, UTextBlock* Text2);

	UFUNCTION() void Button_ExitSettingsPress();
	UFUNCTION() void Button_ExitSettingsRelease();
	UFUNCTION() void Button_ExitSettingsClick();

	UFUNCTION() void Button_YesPress();
	UFUNCTION() void Button_YesRelease();
	UFUNCTION() void Button_YesClick();

	UFUNCTION() void Button_NoPress();
	UFUNCTION() void Button_NoRelease();
	UFUNCTION() void Button_NoClick();


};
