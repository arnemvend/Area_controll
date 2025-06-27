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

	FButtonStyle NewStyle = Button_Start->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Start->SetStyle(NewStyle);
	Button_Start->OnPressed.AddDynamic(this, &UMainMenu::Button_StartPress);
	Button_Start->OnReleased.AddDynamic(this, &UMainMenu::Button_StartRelease);
	Button_Start->OnClicked.AddDynamic(this, &UMainMenu::Button_StartClick);

	NewStyle = Button_Exit->GetStyle();
	NewStyle.Hovered = NewStyle.Normal;
	Button_Exit->SetStyle(NewStyle);
	Button_Exit->OnClicked.AddDynamic(this, &UMainMenu::Button_ExitClick);

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
		}, 0.1f, false);
}




//---------------------------------------------------------------------------//
void UMainMenu::Button_ExitClick()
{
	Border_Quit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button_Start->SetVisibility(ESlateVisibility::Hidden);
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




