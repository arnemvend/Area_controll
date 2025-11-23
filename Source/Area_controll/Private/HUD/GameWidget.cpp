// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/GameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/BackgroundBlur.h"
#include "Components/RadialSlider.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Core/AreaControll_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
//#include "Kismet/GameplayStatics.h"




UGameWidget::UGameWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PauseTextScale = 1.09f;

	ShieldIsActive = false;
	ButtonTowerIsActive = true;
	IsResume = false;
	IsRestart = false;;
	IsRetreat = false;;
	CanChange = false;
	IsFirstCheck = true;
}




bool UGameWidget::Initialize()
{
	Super::Initialize();

	OffStyle = Button_Tower->GetStyle();
	OffStyle.Hovered = OffStyle.Normal;
	Button_Tower->SetStyle(OffStyle);

	OffStyle = Button_Pause->GetStyle();
	OffStyle.Hovered = OffStyle.Normal;
	OffStyle.Pressed = OffStyle.Normal;
	OffStyle.Disabled = OffStyle.Normal;
	Button_Pause->SetStyle(OffStyle);

	//Style of Button_AllShield must be set in last step
	OffStyle = Button_AllShield->GetStyle();
	OffStyle.Hovered = OffStyle.Normal;
	Button_AllShield->SetStyle(OffStyle);

	Button_Tower->OnPressed.AddDynamic(this, &UGameWidget::Button_TowerPress);
	Button_Tower->OnReleased.AddDynamic(this, &UGameWidget::Button_TowerReleas);
	Button_AllShield->OnClicked.AddDynamic(this, &UGameWidget::Button_AllShieldClick);
	Button_Pause->OnClicked.AddDynamic(this, &UGameWidget::Button_PauseClick);
	RadialSlider_Pause->OnValueChanged.AddDynamic(this, &UGameWidget::RadialSliderChanged);
	RadialSlider_Pause->OnMouseCaptureEnd.AddDynamic(this, &UGameWidget::RadialSliderCaptureEnd);
	RadialSlider_Pause->OnMouseCaptureBegin.AddDynamic(this, &UGameWidget::RadialSliderCaptureBegin);

	return true;
}





void UGameWidget::Button_TowerPress()
{
	DeactivatedTowerMenu();
	//waiting for BuildCreator to appear
	if (IsValid(P_Controller))
	{
		P_Controller->CreatorIsHere = true;
	}
}


void UGameWidget::Button_TowerReleas()
{
	if (IsValid(P_Controller))
	{
		P_Controller->SpownCreatorFunc();
	}
}


//on/off shield for all towers in the world
void UGameWidget::Button_AllShieldClick()
{
	if (ButtonTowerIsActive)
	{
		ButtonTowerIsActive = false;
		DeactivatedTowerMenu();
		if (ShieldIsActive)
		{
			ShieldIsVisible = true;
			Button_AllShield->SetStyle(OffStyle);
		}
		else
		{
			ShieldIsVisible = false;
			Button_AllShield->SetStyle(OnStyle);
		}
		OnShieldDelegate.Broadcast(true, ShieldIsVisible);
		//delay before reuse Button_AllShield
		GetWorld()->GetTimerManager().SetTimer(Timer1, [this]()
			{
				ShieldIsActive = !ShieldIsActive;
				ButtonTowerIsActive = true;
			}, 1.1f, false);
	}
}




//click pause and exit-menu
void UGameWidget::Button_PauseClick()
{
	if (!IsValid(P_Controller))
	{
		return;
	}

	if (Overlay_Pause->GetVisibility() == ESlateVisibility::Collapsed)
	{
		DeactivatedTowerMenu();
		Button_Pause->SetIsEnabled(false);

		P_Controller->SetPause(true);

		RadialSlider_Pause->SetValue(RadialSlider_Pause->CustomDefaultValue);
		PauseSliderDMaterial->SetScalarParameterValue
		(TEXT("Rotation Angle"), 1.0f - RadialSlider_Pause->CustomDefaultValue);
		Overlay_Pause->SetVisibility(ESlateVisibility::Visible);
		Blur_Pause->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		P_Controller->SetPause(false);
		RemoveFromParent();
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_MainMenu"));
	}
}




//RadialSlider_Pause logic
void UGameWidget::RadialSliderChanged(float Value)
{
	if (!CanChange)
	{
		return;
	}

	PauseSliderDMaterial->SetScalarParameterValue
	(TEXT("Rotation Angle"), 1.0f - Value);

	if (Value > 0.2f && Value < 0.3f)
	{
		if (!IsResume)
		{
			IsResume = true;
			IsRestart = false;
			IsRetreat = false;

			FontInfo.Size = FontInfo.Size * PauseTextScale;
			TextBlock_Resume->SetFont(FontInfo);
			FontInfo.Size = FontSize;
			TextBlock_restart->SetFont(FontInfo);
			TextBlock_Retreat->SetFont(FontInfo);
		}
	}
	else if (Value > 0.45f && Value < 0.55f)
	{
		if (!IsRestart)
		{
			IsResume = false;
			IsRestart = true;
			IsRetreat = false;

			FontInfo.Size = FontInfo.Size * PauseTextScale;
			TextBlock_restart->SetFont(FontInfo);
			FontInfo.Size = FontSize;
			TextBlock_Resume->SetFont(FontInfo);
			TextBlock_Retreat->SetFont(FontInfo);
		}
	}
	else if (Value > 0.7f && Value < 0.8f)
	{
		if (!IsRetreat)
		{
			IsResume = false;
			IsRestart = false;
			IsRetreat = true;

			FontInfo.Size = FontInfo.Size * PauseTextScale;
			TextBlock_Retreat->SetFont(FontInfo);
			FontInfo.Size = FontSize;
			TextBlock_Resume->SetFont(FontInfo);
			TextBlock_restart->SetFont(FontInfo);
		}
	}
	else if (IsResume || IsRestart || IsRetreat)
	{
		IsResume = false;
		IsRestart = false;
		IsRetreat = false;

		FontInfo.Size = FontSize;
		TextBlock_Resume->SetFont(FontInfo);
		TextBlock_restart->SetFont(FontInfo);
		TextBlock_Retreat->SetFont(FontInfo);
	}
}


void UGameWidget::RadialSliderCaptureEnd()
{
	if (IsResume && IsValid(P_Controller))
	{
		CanChange = false;
		Overlay_Pause->SetVisibility(ESlateVisibility::Collapsed);
		Blur_Pause->SetVisibility(ESlateVisibility::Collapsed);
		RadialSlider_Pause->SetValue(RadialSlider_Pause->CustomDefaultValue);
		PauseSliderDMaterial->SetScalarParameterValue
		(TEXT("Rotation Angle"), 1.0f - RadialSlider_Pause->CustomDefaultValue);
		FontInfo.Size = FontSize;
		TextBlock_Resume->SetFont(FontInfo);
		TextBlock_restart->SetFont(FontInfo);
		TextBlock_Retreat->SetFont(FontInfo);
		Button_Pause->SetIsEnabled(true);
		P_Controller->SetPause(false);
	}
	else if (IsRestart  && IsValid(P_Controller))
	{
		P_Controller->SetPause(false);
		RemoveFromParent();
		P_Controller->ConsoleCommand("RestartLevel");
	}
	else if (IsRetreat  && IsValid(P_Controller))
	{
		P_Controller->SetPause(false);
		RemoveFromParent();
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_MainMenu"));
	}
	else
	{
		CanChange = false;
		RadialSlider_Pause->SetValue(RadialSlider_Pause->CustomDefaultValue);
		PauseSliderDMaterial->SetScalarParameterValue
		(TEXT("Rotation Angle"), 1.0f - RadialSlider_Pause->CustomDefaultValue);
	}
}


void UGameWidget::RadialSliderCaptureBegin()
{
	CanChange = true;
}






//if player use main menu, tower menu will off
void UGameWidget::DeactivatedTowerMenu()
{
	//Delegate for DeTouch func in Tower.cpp
	OnDeTouchDelegate.Broadcast(0);
}




//Update EnergyBar logic
void UGameWidget::UpdateBar()
{
	Text_EnemyEnergy->SetText(FText::AsNumber(GMode->EnemyEnergy));

	Text_YourEnergy->SetText(FText::FromString(FString::Printf
		          (TEXT("%d / %d"), CurrentEnergy, GMode->PlayerMaxEnergy)));

	if (IsValid(GMode) && GMode->PlayerMaxEnergy != 0)
	{
		EnBarDMaterial->SetScalarParameterValue
		        (TEXT("Step"), static_cast<float>(CurrentEnergy) / GMode->PlayerMaxEnergy);
	}
}




//Create Dynamic material
void UGameWidget::SetDMaterialFunc(UImage*& Image, UMaterialInstanceDynamic*& DMaterial)
{
	UObject* ResourseObj = Image->GetBrush().GetResourceObject();
	UMaterialInterface* BrushMaterial = Cast<UMaterialInterface>(ResourseObj);

	if (BrushMaterial)
	{
		DMaterial = UMaterialInstanceDynamic::Create(BrushMaterial, ResourseObj);
	}
	if (DMaterial)
	{
		Image->SetBrushResourceObject(DMaterial);
	    Image->InvalidateLayoutAndVolatility();
	}

	ResourseObj = nullptr;
	BrushMaterial = nullptr;
}





void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	P_Controller = Cast<AAreaControll_PlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(P_Controller) && IsValid(P_Controller->InputComponent))
	{
		P_Controller->InputComponent->BindAction("Quit", IE_Released, 
			this, &UGameWidget::Button_PauseClick).bExecuteWhenPaused = true;
	}

	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		TowerPrice = GInstance->Cn_EnergyPrice;
		Text_TowerPrice->SetText(FText::AsNumber(TowerPrice));
		EnergyLoss = GInstance->ShieldEnergyLoss;

	}
	GInstance = nullptr;

	FontInfo = TextBlock_Resume->GetFont();
	FontSize = TextBlock_Resume->GetFont().Size;


	SetDMaterialFunc(EnergyBarImage, EnBarDMaterial);
	SetDMaterialFunc(Image_Pause, PauseSliderDMaterial);


	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
	{
		Text_FPS->SetText(FText::AsNumber(FMath::TruncToInt(1 / DeltaTime)));
		Text_ForTests->SetText(FText::AsNumber(P_Controller->IsPinch));

		if (IsValid(GMode) && (GMode->PlayerEnergy != CurrentEnergy || IsFirstCheck))
		{
			IsFirstCheck = false;

			CurrentEnergy = GMode->PlayerEnergy;

			UpdateBar();

			if (CurrentEnergy < TowerPrice && /*Button_Tower->GetIsEnabled()*/
				Button_Tower->GetVisibility() == ESlateVisibility::Visible)
			{
				//Button_Tower->SetIsEnabled(false);
				Button_Tower->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CurrentStile = Button_Tower->GetStyle();
				CurrentStile.Normal = CurrentStile.Disabled;
				CurrentStile.Disabled = CurrentStile.Hovered;
				CurrentStile.Hovered = CurrentStile.Normal;
				Button_Tower->SetStyle(CurrentStile);
			}
			else if (CurrentEnergy >= TowerPrice && /*!Button_Tower->GetIsEnabled()*/
				Button_Tower->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
			{
				//Button_Tower->SetIsEnabled(true);
				Button_Tower->SetVisibility(ESlateVisibility::Visible);
				CurrentStile = Button_Tower->GetStyle();
				CurrentStile.Normal = CurrentStile.Disabled;
				CurrentStile.Disabled = CurrentStile.Hovered;
				CurrentStile.Hovered = CurrentStile.Normal;
				Button_Tower->SetStyle(CurrentStile);
			}

			if (CurrentEnergy < EnergyLoss && Button_AllShield->GetIsEnabled())
			{
				ShieldIsActive = false;
				Button_AllShield->SetStyle(OffStyle);
				Button_AllShield->SetIsEnabled(false);
			}
			else if (CurrentEnergy >= EnergyLoss && !Button_AllShield->GetIsEnabled())
			{
				Button_AllShield->SetIsEnabled(true);
			}
		}

	}, 0.2f, true, 0.0f);
}




void UGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DeltaTime = InDeltaTime;
}




void UGameWidget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	//GetWorld()->GetTimerManager().ClearTimer(Timer2);

	if (IsValid(P_Controller) && IsValid(P_Controller->InputComponent))
	{
		P_Controller->InputComponent->RemoveActionBinding("Quit", IE_Released);
	}

	Super::NativeDestruct();
}




//Delegates--------------------------------------------------------------------------->
FOnDeTouchDelegate& UGameWidget::GetOnDeTouchDelegate()
{
	return OnDeTouchDelegate;
}

FOnShieldDelegate& UGameWidget::GetOnShieldDelegate()
{
	return OnShieldDelegate;
}
