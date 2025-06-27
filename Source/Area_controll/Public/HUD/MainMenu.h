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
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Y;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_N;

	UPROPERTY(meta = (BindWidget)) UButton* Button_Start;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget)) UButton* Button_Yes;
	UPROPERTY(meta = (BindWidget)) UButton* Button_No;

	UPROPERTY(meta = (BindWidget)) UBorder* Border_Quit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TextPlayOffset;

	UFUNCTION() void Button_StartPress();
	UFUNCTION() void Button_StartRelease();
	UFUNCTION() void Button_StartClick();

	UFUNCTION() void Button_ExitClick();

	UFUNCTION() void Button_YesPress();
	UFUNCTION() void Button_YesRelease();
	UFUNCTION() void Button_YesClick();

	UFUNCTION() void Button_NoPress();
	UFUNCTION() void Button_NoRelease();
	UFUNCTION() void Button_NoClick();
};
