// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/MainMenu.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Core/MainMenu_PlayerController.h"
#include "Kismet/GameplayStatics.h"


bool UMainMenu::Initialize()
{
	Super::Initialize();

	TextPlayOffset = 7.0f;
	TextSettingsOffset = 6.0f;
	TextExitOffset = 5.0f;

	FButtonStyle NewStyle = Button_Start->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Start->SetStyle(NewStyle);
	Button_Start->OnPressed.AddDynamic(this, &UMainMenu::Button_StartPress);
	Button_Start->OnReleased.AddDynamic(this, &UMainMenu::Button_StartRelease);
	Button_Start->OnClicked.AddDynamic(this, &UMainMenu::Button_StartClick);

	NewStyle = Button_Exit->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Exit->SetStyle(NewStyle);
	Button_Exit->OnPressed.AddDynamic(this, &UMainMenu::Button_ExitPress);
	Button_Exit->OnReleased.AddDynamic(this, &UMainMenu::Button_ExitRelease);
	Button_Exit->OnClicked.AddDynamic(this, &UMainMenu::Button_ExitClick);

	NewStyle = Button_Settings->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Settings->SetStyle(NewStyle);
	Button_Settings->OnPressed.AddDynamic(this, &UMainMenu::Button_SettingsPress);
	Button_Settings->OnReleased.AddDynamic(this, &UMainMenu::Button_SettingsRelease);
	Button_Settings->OnClicked.AddDynamic(this, &UMainMenu::Button_SettingsClick);

	NewStyle = Button_Low->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Low->SetStyle(NewStyle);
	Button_Low->OnPressed.AddDynamic(this, &UMainMenu::Button_LowPress);

	NewStyle = Button_Medium->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Medium->SetStyle(NewStyle);
	Button_Medium->OnPressed.AddDynamic(this, &UMainMenu::Button_MediumPress);

	NewStyle = Button_High->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_High->SetStyle(NewStyle);
	Button_High->OnPressed.AddDynamic(this, &UMainMenu::Button_HighPress);

	NewStyle = Button_ExitSettings->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_ExitSettings->SetStyle(NewStyle);
	Button_ExitSettings->OnPressed.AddDynamic(this, &UMainMenu::Button_ExitSettingsPress);
	Button_ExitSettings->OnReleased.AddDynamic(this, &UMainMenu::Button_ExitSettingsRelease);
	Button_ExitSettings->OnClicked.AddDynamic(this, &UMainMenu::Button_ExitSettingsClick);

	NewStyle = Button_Yes->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Pressed = NewStyle.Normal;
	Button_Yes->OnPressed.AddDynamic(this, &UMainMenu::Button_YesPress);
	Button_Yes->OnReleased.AddDynamic(this, &UMainMenu::Button_YesRelease);
	Button_Yes->OnClicked.AddDynamic(this, &UMainMenu::Button_YesClick);

	NewStyle = Button_No->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	NewStyle.Pressed = NewStyle.Normal;
	Button_No->OnPressed.AddDynamic(this, &UMainMenu::Button_NoPress);
	Button_No->OnReleased.AddDynamic(this, &UMainMenu::Button_NoRelease);
	Button_No->OnClicked.AddDynamic(this, &UMainMenu::Button_NoClick);


	return true;
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_StartPress()
{
	FVector2D Translation = Text_Play->GetRenderTransform().Translation;
	Translation.Y += TextPlayOffset;
	Text_Play->SetRenderTranslation(Translation);
}


void UMainMenu::Button_StartRelease()
{
	FVector2D Translation = Text_Play->GetRenderTransform().Translation;
	Translation.Y -= TextPlayOffset;
	Text_Play->SetRenderTranslation(Translation);
}


void UMainMenu::Button_StartClick()
{
	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"));
		}, 0.05f, false);
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_ExitPress()
{
	FVector2D Translation = Text_Exit->GetRenderTransform().Translation;
	Translation.Y += TextSettingsOffset;
	Text_Exit->SetRenderTranslation(Translation);
}


void UMainMenu::Button_ExitRelease()
{
	FVector2D Translation = Text_Exit->GetRenderTransform().Translation;
	Translation.Y -= TextSettingsOffset;
	Text_Exit->SetRenderTranslation(Translation);
}


void UMainMenu::Button_ExitClick()
{
	Border_Quit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_Start->SetVisibility(ESlateVisibility::Hidden);
	Button_Exit->SetVisibility(ESlateVisibility::Hidden);
	Button_Settings->SetVisibility(ESlateVisibility::Hidden);
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_SettingsPress()
{
	FVector2D Translation = Text_Settings->GetRenderTransform().Translation;
	Translation.Y += TextSettingsOffset;
	Text_Settings->SetRenderTranslation(Translation);
}


void UMainMenu::Button_SettingsRelease()
{
	FVector2D Translation = Text_Settings->GetRenderTransform().Translation;
	Translation.Y -= TextSettingsOffset;
	Text_Settings->SetRenderTranslation(Translation);
}


void UMainMenu::Button_SettingsClick()
{
	Border_Settings->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_Start->SetVisibility(ESlateVisibility::Hidden);
	Button_Exit->SetVisibility(ESlateVisibility::Hidden);
	Button_Settings->SetVisibility(ESlateVisibility::Hidden);
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_LowPress()
{
	Button_Low->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FButtonStyle NewStyle = Button_Low->GetStyle();
	NewStyle.Normal = NewStyle.Pressed;
	Button_Low->SetStyle(NewStyle);
	Button_Low->ForceLayoutPrepass();

	FVector2D Translation = Text_Low->GetRenderTransform().Translation;
	Translation.Y += TextSettingsOffset;
	Text_Low->SetRenderTranslation(Translation);

	VideoSettingsReClick(Button_Medium, Button_High, Text_Medium, Text_High);
}


void UMainMenu::Button_MediumPress()
{
	Button_Medium->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FButtonStyle NewStyle = Button_Medium->GetStyle();
	NewStyle.Normal = NewStyle.Pressed;
	Button_Medium->SetStyle(NewStyle);
	Button_Medium->ForceLayoutPrepass();

	FVector2D Translation = Text_Medium->GetRenderTransform().Translation;
	Translation.Y += TextSettingsOffset;
	Text_Medium->SetRenderTranslation(Translation);

	VideoSettingsReClick(Button_Low, Button_High, Text_Low, Text_High);
}


void UMainMenu::Button_HighPress()
{
	Button_High->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FButtonStyle NewStyle = Button_High->GetStyle();
	NewStyle.Normal = NewStyle.Pressed;
	Button_High->SetStyle(NewStyle);
	Button_High->ForceLayoutPrepass();

	FVector2D Translation = Text_High->GetRenderTransform().Translation;
	Translation.Y += TextSettingsOffset;
	Text_High->SetRenderTranslation(Translation);

	VideoSettingsReClick(Button_Low, Button_Medium, Text_Low, Text_Medium);
}


void UMainMenu::VideoSettingsReClick(UButton* Button1, UButton* Button2, UTextBlock* Text1, UTextBlock* Text2)
{
	UButton* NewButton = nullptr;
	UTextBlock* NewText = nullptr;

	if (Button1->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		NewButton = Button1;
		NewText = Text1;
	}
	else if (Button2->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		NewButton = Button2;
		NewText = Text2;
	}

	if (!(NewButton || NewText))
	{
		return;
	}

	FButtonStyle NewStyle = NewButton->GetStyle();
	NewStyle.Normal = NewStyle.Hovered;
	NewButton->SetStyle(NewStyle);
	NewButton->ForceLayoutPrepass();
	NewButton->SetVisibility(ESlateVisibility::Visible);
	NewButton = nullptr;

	FVector2D Translation = NewText->GetRenderTransform().Translation;
	Translation.Y -= TextSettingsOffset;
	NewText->SetRenderTranslation(Translation);
	NewText = nullptr;
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_ExitSettingsPress()
{
	FVector2D Translation = Text_ExitSettings->GetRenderTransform().Translation;
	Translation.Y += TextExitOffset;
	Text_ExitSettings->SetRenderTranslation(Translation);
}


void UMainMenu::Button_ExitSettingsRelease()
{
	FVector2D Translation = Text_ExitSettings->GetRenderTransform().Translation;
	Translation.Y -= TextExitOffset;
	Text_ExitSettings->SetRenderTranslation(Translation);
}


void UMainMenu::Button_ExitSettingsClick()
{
	Border_Settings->SetVisibility(ESlateVisibility::Collapsed);
	Button_Start->SetVisibility(ESlateVisibility::Visible);
	Button_Exit->SetVisibility(ESlateVisibility::Visible);
	Button_Settings->SetVisibility(ESlateVisibility::Visible);
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_YesPress()
{
	FVector2D Translation = Text_Y->GetRenderTransform().Translation;
	Translation.Y += TextPlayOffset;
	Text_Y->SetRenderTranslation(Translation);
}


void UMainMenu::Button_YesRelease()
{
	FVector2D Translation = Text_Y->GetRenderTransform().Translation;
	Translation.Y -= TextPlayOffset;
	Text_Y->SetRenderTranslation(Translation);
}


void UMainMenu::Button_YesClick()
{
	if (IsValid(PController))
	{
		PController->ConsoleCommand("quit");
	}
}





//---------------------------------------------------------------------------//
void UMainMenu::Button_NoPress()
{
	FVector2D Translation = Text_N->GetRenderTransform().Translation;
	Translation.Y += TextPlayOffset;
	Text_N->SetRenderTranslation(Translation);
}


void UMainMenu::Button_NoRelease()
{
	FVector2D Translation = Text_N->GetRenderTransform().Translation;
	Translation.Y -= TextPlayOffset;
	Text_N->SetRenderTranslation(Translation);
}


void UMainMenu::Button_NoClick()
{
	Border_Quit->SetVisibility(ESlateVisibility::Collapsed);
	Button_Start->SetVisibility(ESlateVisibility::Visible);
	Button_Exit->SetVisibility(ESlateVisibility::Visible);
	Button_Settings->SetVisibility(ESlateVisibility::Visible);
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




