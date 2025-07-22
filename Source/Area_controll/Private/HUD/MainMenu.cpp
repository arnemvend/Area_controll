// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/MainMenu.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "NiagaraSystemWidget.h"
#include "NiagaraUIComponent.h"
#include "Animation/WidgetAnimation.h" 
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Core/MainMenu_GameMode.h"
#include "Core/MainMenu_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"




UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MainMenuAnimSpeed = 1.0f;
	SettingsAnimSpeed = 1.0f;
	QuitAnimSpeed = 1.0f;
	QuitTextScale = FVector2D(1.3f, 1.3f);
}


bool UMainMenu::Initialize()
{
	Super::Initialize();

	FButtonStyle NewStyle = Button_Start->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Normal;
	Button_Start->SetStyle(NewStyle);
	Button_Start->OnClicked.AddDynamic(this, &UMainMenu::Button_StartClick);

	NewStyle = Button_Exit->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Normal;
	Button_Exit->SetStyle(NewStyle);
	Button_Exit->OnClicked.AddDynamic(this, &UMainMenu::Button_ExitClick);

	NewStyle = Button_Settings->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Normal;
	Button_Settings->SetStyle(NewStyle);
	Button_Settings->OnClicked.AddDynamic(this, &UMainMenu::Button_SettingsClick);

	NewStyle = Button_Low->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Pressed;
	Button_Low->SetStyle(NewStyle);
	Button_Low->OnPressed.AddDynamic(this, &UMainMenu::Button_LowPress);

	NewStyle = Button_High->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Pressed;
	Button_High->SetStyle(NewStyle);
	Button_High->OnPressed.AddDynamic(this, &UMainMenu::Button_HighPress);

	NewStyle = Button_ExitSettings->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Disabled = NewStyle.Pressed;
	Button_ExitSettings->SetStyle(NewStyle);
	Button_ExitSettings->OnClicked.AddDynamic(this, &UMainMenu::Button_ExitSettingsClick);

	FCheckBoxStyle CheckBoxStyle = CheckBox_Effects->GetWidgetStyle();
	CheckBoxStyle.UncheckedHoveredImage = CheckBoxStyle.UncheckedImage;
	CheckBoxStyle.UncheckedPressedImage = CheckBoxStyle.UncheckedImage;
	CheckBoxStyle.CheckedHoveredImage = CheckBoxStyle.CheckedImage;
	CheckBoxStyle.CheckedPressedImage = CheckBoxStyle.CheckedImage;
	CheckBox_Effects->SetWidgetStyle(CheckBoxStyle);

	CheckBoxStyle = CheckBox_Music->GetWidgetStyle();
	CheckBoxStyle.UncheckedHoveredImage = CheckBoxStyle.UncheckedImage;
	CheckBoxStyle.UncheckedPressedImage = CheckBoxStyle.UncheckedImage;
	CheckBoxStyle.CheckedHoveredImage = CheckBoxStyle.CheckedImage;
	CheckBoxStyle.CheckedPressedImage = CheckBoxStyle.CheckedImage;
	CheckBox_Music->SetWidgetStyle(CheckBoxStyle);

	FSliderStyle SliderStyle = Slider_YesNo->GetWidgetStyle();
	SliderStyle.HoveredBarImage = SliderStyle.NormalBarImage;
	SliderStyle.DisabledBarImage = SliderStyle.NormalBarImage;
	SliderStyle.HoveredThumbImage = SliderStyle.NormalThumbImage;
	SliderStyle.DisabledThumbImage = SliderStyle.NormalThumbImage;
	Slider_YesNo->SetWidgetStyle(SliderStyle);
	Slider_YesNo->SetValue(0.5f);
	Slider_YesNo->OnValueChanged.AddDynamic(this, &UMainMenu::Slider_YesNoProgressChanged);
	Slider_YesNo->OnMouseCaptureEnd.AddDynamic(this, &UMainMenu::Slider_YesNoMCaptureEnd);

	return true;
}




//---------------------------------------------------------------------------//
void UMainMenu::Finish_ForwardMainMenuAnim()
{
	Button_Start->SetIsEnabled(true);
	Button_Exit->SetIsEnabled(true);
	Button_Settings->SetIsEnabled(true);
}


void UMainMenu::Finish_ReverseMainMenuAnim()
{
	VerticalBox_Main->SetVisibility(ESlateVisibility::Collapsed);

	if (ToSettings)
	{
		PlayAnimation(SettingsAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, SettingsAnimSpeed);
		GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
			{
				Border_Settings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Button_Low->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Button_High->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Button_ExitSettings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CheckBox_Effects->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CheckBox_Music->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}, 0.01f, false);
	}
	else
	{
		PlayAnimation(QuitAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, QuitAnimSpeed);
		GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
			{
				Border_Quit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				NSystemSlider->ActivateSystem(true);
			}, 0.01f, false);
	}
}


void UMainMenu::ToNewLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"));
}


void UMainMenu::Finish_ForwardSettingsAnim()
{
	Button_Low->SetVisibility(ESlateVisibility::Visible);
	Button_High->SetVisibility(ESlateVisibility::Visible);
	Button_ExitSettings->SetVisibility(ESlateVisibility::Visible);
	CheckBox_Effects->SetVisibility(ESlateVisibility::Visible);
	CheckBox_Music->SetVisibility(ESlateVisibility::Visible);
}


void UMainMenu::Finish_ReverseSettingsAnim()
{
	Border_Settings->SetVisibility(ESlateVisibility::Collapsed);

	Button_Start->SetIsEnabled(false);
	Button_Exit->SetIsEnabled(false);
	Button_Settings->SetIsEnabled(false);

	PlayAnimation(MainMenuAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, MainMenuAnimSpeed);
	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
		{
			VerticalBox_Main->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}, 0.01f, false);
}


void UMainMenu::Finish_ForwardQuitAnim()
{
	Slider_YesNo->SetIsEnabled(true);
}


void UMainMenu::Finish_ReverseQuitAnim()
{
	NSystemSlider->DeactivateSystem();
	Border_Quit->SetVisibility(ESlateVisibility::Collapsed);
	Slider_YesNo->SetValue(0.5f);

	PlayAnimation(MainMenuAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, MainMenuAnimSpeed);
	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
		{
			VerticalBox_Main->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Button_Start->SetIsEnabled(false);
			Button_Exit->SetIsEnabled(false);
			Button_Settings->SetIsEnabled(false);
		}, 0.01f, false);
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_StartClick()
{
	UnbindFromAnimationFinished(MainMenuAnim, MainMenuEvent);
	MainMenuEvent.Unbind();
	MainMenuEvent.BindDynamic(this, &UMainMenu::UMainMenu::ToNewLevel);
	BindToAnimationFinished(MainMenuAnim, MainMenuEvent);

	Button_Start->SetIsEnabled(false);
	Button_Exit->SetIsEnabled(false);
	Button_Settings->SetIsEnabled(false);

	PlayAnimation(MainMenuAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, MainMenuAnimSpeed);
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_ExitClick()
{
	if (Border_Quit->GetVisibility() == ESlateVisibility::Collapsed)
	{
		ToSettings = false;

		Button_Start->SetIsEnabled(false);
		Button_Exit->SetIsEnabled(false);
		Button_Settings->SetIsEnabled(false);

		NSystemSlider->GetNiagaraComponent()->SetVariablePosition(FName("StartPosition"),
			FVector(-(NSystemSlider->GetDesiredWidgetSize().X / 2), 0.0f, 0.0f));
		NiagaraUpdate(Slider_YesNo->GetValue());

		UnbindFromAnimationFinished(MainMenuAnim, MainMenuEvent);
		MainMenuEvent.Unbind();
		MainMenuEvent.BindDynamic(this, &UMainMenu::Finish_ReverseMainMenuAnim);
		BindToAnimationFinished(MainMenuAnim, MainMenuEvent);

		UnbindFromAnimationFinished(QuitAnim, QuitEvent);
		QuitEvent.Unbind();
		QuitEvent.BindDynamic(this, &UMainMenu::Finish_ForwardQuitAnim);
		BindToAnimationFinished(QuitAnim, QuitEvent);

		PlayAnimation(MainMenuAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, MainMenuAnimSpeed);
	}
	else if (IsValid(PController))
	{
		PController->ConsoleCommand("quit");
	}
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_SettingsClick()
{
	ToSettings = true;

	Button_Start->SetIsEnabled(false);
	Button_Exit->SetIsEnabled(false);
	Button_Settings->SetIsEnabled(false);

	UnbindFromAnimationFinished(MainMenuAnim, MainMenuEvent);
	MainMenuEvent.Unbind();
	MainMenuEvent.BindDynamic(this, &UMainMenu::Finish_ReverseMainMenuAnim);
	BindToAnimationFinished(MainMenuAnim, MainMenuEvent);

	UnbindFromAnimationFinished(SettingsAnim, SettingsEvent);
	SettingsEvent.Unbind();
	SettingsEvent.BindDynamic(this, &UMainMenu::Finish_ForwardSettingsAnim);
	BindToAnimationFinished(SettingsAnim, SettingsEvent);

	PlayAnimation(MainMenuAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, MainMenuAnimSpeed);
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_LowPress()
{
	if (IsValid(GMode))
	{
		GMode->SetSettings(0);
	}

	Button_Low->SetIsEnabled(false);
	Button_High->SetIsEnabled(true);
}


void UMainMenu::Button_HighPress()
{
	if (IsValid(GMode))
	{
		GMode->SetSettings(2);
	}

	Button_Low->SetIsEnabled(true);
	Button_High->SetIsEnabled(false);
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_ExitSettingsClick()
{
	Button_Low->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_High->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_ExitSettings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UnbindFromAnimationFinished(MainMenuAnim, MainMenuEvent);
	MainMenuEvent.Unbind();
	MainMenuEvent.BindDynamic(this, &UMainMenu::Finish_ForwardMainMenuAnim);
	BindToAnimationFinished(MainMenuAnim, MainMenuEvent);

	UnbindFromAnimationFinished(SettingsAnim, SettingsEvent);
	SettingsEvent.Unbind();
	SettingsEvent.BindDynamic(this, &UMainMenu::Finish_ReverseSettingsAnim);
	BindToAnimationFinished(SettingsAnim, SettingsEvent);

	PlayAnimation(SettingsAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, SettingsAnimSpeed);
}




//---------------------------------------------------------------------------//
void UMainMenu::Slider_YesNoProgressChanged(float Value)
{
	NiagaraUpdate(Value);

	if (Value >= 0.9f)
	{
		Text_Y->SetRenderScale(QuitTextScale);
	}
	else if (Value <= 0.1f)
	{
		Text_N->SetRenderScale(QuitTextScale);
	}
	else
	{
		Text_Y->SetRenderScale(FVector2D(1.0f, 1.0f));
		Text_N->SetRenderScale(FVector2D(1.0f, 1.0f));
	}
}


void UMainMenu::Slider_YesNoMCaptureEnd()
{
	if (Slider_YesNo->GetValue() >= 0.9f)
	{
		if (IsValid(PController))
		{
			PController->ConsoleCommand("quit");
		}
	}
	else if (Slider_YesNo->GetValue() <= 0.1f)
	{
		Slider_YesNo->SetIsEnabled(false);

		Text_Y->SetRenderScale(FVector2D(1.0f, 1.0f));
		Text_N->SetRenderScale(FVector2D(1.0f, 1.0f));

		UnbindFromAnimationFinished(MainMenuAnim, MainMenuEvent);
		MainMenuEvent.Unbind();
		MainMenuEvent.BindDynamic(this, &UMainMenu::Finish_ForwardMainMenuAnim);
		BindToAnimationFinished(MainMenuAnim, MainMenuEvent);

		UnbindFromAnimationFinished(QuitAnim, QuitEvent);
		QuitEvent.Unbind();
		QuitEvent.BindDynamic(this, &UMainMenu::Finish_ReverseQuitAnim);
		BindToAnimationFinished(QuitAnim, QuitEvent);

		PlayAnimation(QuitAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, QuitAnimSpeed);
	}
	else
	{
		Slider_YesNo->SetValue(0.5f);

		Text_Y->SetRenderScale(FVector2D(1.0f, 1.0f));
		Text_N->SetRenderScale(FVector2D(1.0f, 1.0f));
	}
}


void UMainMenu::NiagaraUpdate(float Offset)
{
	const float TotalWidth = NSystemSlider->GetDesiredWidgetSize().X;
	const float EndPositionX = TotalWidth * (Offset - 0.5f);
	const float MidPositionX = (EndPositionX / 2.0f) - (TotalWidth / 4.0f);
	//The Pythagorean theorem
	const float HalfWidth = TotalWidth / 2.0f;
	const float TriangleHeight = FMath::Sqrt(
		FMath::Square(HalfWidth) - FMath::Square((EndPositionX / 2.0f) + (TotalWidth / 4.0f)));

	NSystemSlider->GetNiagaraComponent()->SetVariablePosition(FName("EndPosition"), 
		FVector(EndPositionX, 0.0f, 0.0f));
	NSystemSlider->GetNiagaraComponent()->SetVariablePosition(FName("MidPosition"), 
		FVector(MidPositionX, 0.0f, TriangleHeight));
}





//---------------------------------------------------------------------------//
void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PController = Cast<AMainMenu_PlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PController) && IsValid(PController->InputComponent))
	{
		PController->InputComponent->BindAction("Quit", IE_Released, this, &UMainMenu::Button_ExitClick);
	}

	GMode = Cast<AMainMenu_GameMode>(GetWorld()->GetAuthGameMode());

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	int Type = Settings->GetViewDistanceQuality();

	switch (Type)
	{
	case 0:
		Button_LowPress();
		break;

	case 2:
		Button_HighPress();
		break;

	default: 
		Button_HighPress();
		return;
	}
}




void UMainMenu::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer0);

	if (IsValid(PController) && IsValid(PController->InputComponent))
	{
		PController->InputComponent->RemoveActionBinding("Quit", IE_Released);
	}
	
	Super::NativeDestruct();
}




