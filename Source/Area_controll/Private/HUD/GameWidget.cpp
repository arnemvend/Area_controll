// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/GameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Core/AreaControll_PlayerController.h"
#include "Kismet/GameplayStatics.h"




bool UGameWidget::Initialize()
{
	Super::Initialize();

	Button_Tower->OnPressed.AddDynamic(this, &UGameWidget::Button_TowerPress);
	Button_Tower->OnReleased.AddDynamic(this, &UGameWidget::Button_TowerReleas);
	Button_AllShield->OnClicked.AddDynamic(this, &UGameWidget::Button_AllShieldClick);

	return true;
}





void UGameWidget::Button_TowerPress()
{
	DeactivatedTowerMenu();
	//waiting for BuildCreator to appear
	P_Controller->CreatorIsHere = true;
}


void UGameWidget::Button_TowerReleas()
{
	P_Controller->SpownCreatorFunc();
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
		}
		else
		{
			ShieldIsVisible = false;
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


//if player use main menu, tower menu will off
void UGameWidget::DeactivatedTowerMenu()
{
	//Delegate for DeTouch func in Tower.cpp
	OnDeTouchDelegate.Broadcast(0);
}




void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	P_Controller = Cast<AAreaControll_PlayerController>(GetWorld()->GetFirstPlayerController());
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		TowerPrice = GInstance->Cn_EnergyPrice;
		//Text_Cn_EnergyPrice->SetText(FText::AsNumber(TowerPrice));
	}
	GInstance = nullptr;

	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
	{
		Text_FPS->SetText(FText::AsNumber(FMath::TruncToInt(1 / DeltaTime)));

		Text_EnemyEnergy->SetText(FText::AsNumber(GMode->EnemyEnergy));
		Text_YourEnergy->SetText(FText::AsNumber(GMode->PlayerEnergy));

		if (GMode->PlayerEnergy < TowerPrice && Button_Tower->GetIsEnabled())
		{
			Button_Tower->SetIsEnabled(false);
		}
		else if (GMode->PlayerEnergy >= TowerPrice && !Button_Tower->GetIsEnabled())
		{
			Button_Tower->SetIsEnabled(true);
		}
	}, 0.2f, true);
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
