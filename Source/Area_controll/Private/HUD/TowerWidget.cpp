// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/TowerWidget.h"
#include "Components/Button.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower/Tower.h"



bool UTowerWidget::Initialize()
{
	Super::Initialize();

	Button_shield->OnClicked.AddDynamic(this, &UTowerWidget::Button_shieldClick);
	Button_destroy->OnClicked.AddDynamic(this, &UTowerWidget::Button_destroyClick);
	Button_Repeater->OnClicked.AddDynamic(this, &UTowerWidget::Button_RepeaterClick);
	Button_guns->OnClicked.AddDynamic(this, &UTowerWidget::Button_gunsClick);
	Button_UpGun->OnClicked.AddDynamic(this, &UTowerWidget::Button_UpGunClick);
	Button_MidGun->OnClicked.AddDynamic(this, &UTowerWidget::Button_MidGunClick);
	Button_LowGun->OnClicked.AddDynamic(this, &UTowerWidget::Button_LowGunClick);
	Button_gun00->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun00Click);
	Button_gun01->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun01Click);
	Button_gun10->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun10Click);
	Button_gun11->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun11Click);
	Button_gun20->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun20Click);
	Button_gun21->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun21Click);
	Button_Delete0->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete0Click);
	Button_Delete1->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete1Click);
	Button_Delete2->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete2Click);

	return true;
}




void UTowerWidget::StartEvent()
{
	if (IsValid(MyTower) && MyTower->ActorHasTag(TEXT("Main")))
	{
		Button_Repeater->SetIsEnabled(false);
		Button_Repeater->SetVisibility(ESlateVisibility::Collapsed);
		Button_destroy->SetIsEnabled(false);
		Button_destroy->SetVisibility(ESlateVisibility::Collapsed);
		Button_guns->SetIsEnabled(false);
		Button_guns->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(MyTower) && MyTower->IsRepeater)
	{
		Button_Repeater->SetIsEnabled(false);
		Button_Repeater->SetVisibility(ESlateVisibility::Collapsed);
		Button_shield->SetIsEnabled(false);
		Button_shield->SetVisibility(ESlateVisibility::Collapsed);
		Button_guns->SetIsEnabled(false);
		Button_guns->SetVisibility(ESlateVisibility::Collapsed);
	}
}


//enables repeater mode for the tower
void UTowerWidget::Button_RepeaterClick()
{
	if (MyTower)
	{
		MyTower->Repeater();
		Button_shield->SetIsEnabled(false);
		Button_shield->SetVisibility(ESlateVisibility::Collapsed);
		Button_Repeater->SetIsEnabled(false);
		Button_Repeater->SetVisibility(ESlateVisibility::Collapsed);
		MyTower->DeTouch(0);
	}
}


//destroy the tower
void UTowerWidget::Button_destroyClick()
{
	if (IsValid(MyTower))
	{
		MyTower->RunDelete();
	}
}


//enables/disables shield
void UTowerWidget::Button_shieldClick()
{
	if (IsValid(MyTower))
	{
		MyTower->ShieldCreate();
	}
}


//TowerGuns menu enables/disables
void UTowerWidget::Button_gunsClick()
{
	if (Button_UpGun->IsVisible())
	{
		Button_UpGun->SetVisibility(ESlateVisibility::Hidden);
		Button_MidGun->SetVisibility(ESlateVisibility::Hidden);
		Button_LowGun->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Button_UpGun->SetVisibility(ESlateVisibility::Visible);
		Button_MidGun->SetVisibility(ESlateVisibility::Visible);
		Button_LowGun->SetVisibility(ESlateVisibility::Visible);
		Button_gun00->SetVisibility(ESlateVisibility::Hidden);
		Button_gun01->SetVisibility(ESlateVisibility::Hidden);
		Button_gun10->SetVisibility(ESlateVisibility::Hidden);
		Button_gun11->SetVisibility(ESlateVisibility::Hidden);
		Button_gun20->SetVisibility(ESlateVisibility::Hidden);
		Button_gun21->SetVisibility(ESlateVisibility::Hidden);
		Button_Delete0->SetVisibility(ESlateVisibility::Collapsed);
		Button_Delete1->SetVisibility(ESlateVisibility::Collapsed);
		Button_Delete2->SetVisibility(ESlateVisibility::Collapsed);
	}
}


//Gun of 0 type menu
void UTowerWidget::Button_UpGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->UpGun) &&
		IsValid(MyTower->UpGun->GetChildActor());

	if (!Button_gun00->IsVisible())
	{
		Button_gun00->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		Button_gun01->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		Button_gun10->SetVisibility(ESlateVisibility::Hidden);
		Button_gun11->SetVisibility(ESlateVisibility::Hidden);

		Button_gun20->SetVisibility(ESlateVisibility::Hidden);
		Button_gun21->SetVisibility(ESlateVisibility::Hidden);

		if (Button_gun00->IsVisible())
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer1);
			GetWorld()->GetTimerManager().ClearTimer(Timer2);
			GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
				{
					if (IsValid(GInstance) && IsValid(GMode))
					{
						bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG00_EnergyPrice;
						Button_gun00->SetIsEnabled(bGoodPrice);

						bGoodPrice = GMode->PlayerEnergy >= GInstance->TG01_EnergyPrice;
						Button_gun01->SetIsEnabled(bGoodPrice);

						UE_LOG(LogTemp, Warning, TEXT("Timer0"));
					}
				}, 0.2f, true, 0.0f);
		}
	}
	else
	{
		Button_gun00->SetVisibility(ESlateVisibility::Hidden);
		Button_gun01->SetVisibility(ESlateVisibility::Hidden);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);
	}
	if (!Button_Delete0->IsVisible())
	{
		Button_Delete0->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		Button_Delete1->SetVisibility(ESlateVisibility::Collapsed);

		Button_Delete2->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Button_Delete0->SetVisibility(ESlateVisibility::Collapsed);
	}
}



//Gun of 1 type menu
void UTowerWidget::Button_MidGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->MidGun) &&
		IsValid(MyTower->MidGun->GetChildActor());

	if (!Button_gun10->IsVisible())
	{
		Button_gun10->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		Button_gun11->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		Button_gun00->SetVisibility(ESlateVisibility::Hidden);
		Button_gun01->SetVisibility(ESlateVisibility::Hidden);

		Button_gun20->SetVisibility(ESlateVisibility::Hidden);
		Button_gun21->SetVisibility(ESlateVisibility::Hidden);

		if (Button_gun10->IsVisible())
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer0);
			GetWorld()->GetTimerManager().ClearTimer(Timer2);
			GetWorld()->GetTimerManager().SetTimer(Timer1, [this]()
				{
					if (IsValid(GInstance) && IsValid(GMode))
					{
						bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG10_EnergyPrice;
						Button_gun10->SetIsEnabled(bGoodPrice);

						bGoodPrice = GMode->PlayerEnergy >= GInstance->TG11_EnergyPrice;
						Button_gun11->SetIsEnabled(bGoodPrice);

						UE_LOG(LogTemp, Warning, TEXT("Timer1"));
					}
				}, 0.2f, true, 0.0f);
		}
	}
	else
	{
		Button_gun10->SetVisibility(ESlateVisibility::Hidden);
		Button_gun11->SetVisibility(ESlateVisibility::Hidden);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);
	}
	if (!Button_Delete1->IsVisible())
	{
		Button_Delete1->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		Button_Delete0->SetVisibility(ESlateVisibility::Collapsed);

		Button_Delete2->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Button_Delete1->SetVisibility(ESlateVisibility::Collapsed);
	}
}


//Gun of 2 type menu
void UTowerWidget::Button_LowGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->LowGun) &&
		IsValid(MyTower->LowGun->GetChildActor());

	if (!Button_gun20->IsVisible())
	{
		Button_gun20->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		Button_gun21->SetVisibility(bHasActiveGun ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		Button_gun00->SetVisibility(ESlateVisibility::Hidden);
		Button_gun01->SetVisibility(ESlateVisibility::Hidden);

		Button_gun10->SetVisibility(ESlateVisibility::Hidden);
		Button_gun11->SetVisibility(ESlateVisibility::Hidden);

		if (Button_gun20->IsVisible())
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer0);
			GetWorld()->GetTimerManager().ClearTimer(Timer1);
			GetWorld()->GetTimerManager().SetTimer(Timer2, [this]()
				{
					if (IsValid(GInstance) && IsValid(GMode))
					{
						bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG20_EnergyPrice;
						Button_gun20->SetIsEnabled(bGoodPrice);

						bGoodPrice = GMode->PlayerEnergy >= GInstance->TG21_EnergyPrice;
						Button_gun21->SetIsEnabled(bGoodPrice);

						UE_LOG(LogTemp, Warning, TEXT("Timer2"));
					}
				}, 0.2f, true, 0.0f);
		}
	}
	else
	{
		Button_gun20->SetVisibility(ESlateVisibility::Hidden);
		Button_gun21->SetVisibility(ESlateVisibility::Hidden);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);
		
	}
	if (!Button_Delete2->IsVisible())
	{
		Button_Delete2->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		Button_Delete0->SetVisibility(ESlateVisibility::Collapsed);

		Button_Delete1->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Button_Delete2->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UTowerWidget::Button_gun00Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->UpGun) &&
		!IsValid(MyTower->UpGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(0, 0);
	}

	Button_gun00->SetVisibility(ESlateVisibility::Hidden);
	Button_gun01->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer0);
}


void UTowerWidget::Button_gun01Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->UpGun) &&
		!IsValid(MyTower->UpGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(0, 1);
	}

	Button_gun00->SetVisibility(ESlateVisibility::Hidden);
	Button_gun01->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer0);
}


void UTowerWidget::Button_gun10Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->MidGun) &&
		!IsValid(MyTower->MidGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(1, 0);
	}

	Button_gun10->SetVisibility(ESlateVisibility::Hidden);
	Button_gun11->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
}


void UTowerWidget::Button_gun11Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->MidGun) &&
		!IsValid(MyTower->MidGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(1, 1);
	}

	Button_gun10->SetVisibility(ESlateVisibility::Hidden);
	Button_gun11->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
}


void UTowerWidget::Button_gun20Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->LowGun) &&
		!IsValid(MyTower->LowGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(2, 0);
	}

	Button_gun20->SetVisibility(ESlateVisibility::Hidden);
	Button_gun21->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);
}


void UTowerWidget::Button_gun21Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->LowGun) &&
		!IsValid(MyTower->LowGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->CreateGun(2, 1);
	}

	Button_gun20->SetVisibility(ESlateVisibility::Hidden);
	Button_gun21->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);
}


void UTowerWidget::Button_Delete0Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->UpGun) &&
		IsValid(MyTower->UpGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->DeleteGun(0);
		Button_Delete0->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UTowerWidget::Button_Delete1Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->MidGun) &&
		IsValid(MyTower->MidGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->DeleteGun(1);
		Button_Delete1->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UTowerWidget::Button_Delete2Click()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->LowGun) &&
		IsValid(MyTower->LowGun->GetChildActor());

	if (bHasActiveGun)
	{
		MyTower->DeleteGun(2);
		Button_Delete2->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UTowerWidget::Reset()
{
	Button_Repeater->SetIsEnabled(true);
	Button_Repeater->SetVisibility(ESlateVisibility::Visible);
	Button_destroy->SetIsEnabled(true);
	Button_destroy->SetVisibility(ESlateVisibility::Visible);
	Button_shield->SetIsEnabled(true);
	Button_shield->SetVisibility(ESlateVisibility::Visible);
	Button_guns->SetIsEnabled(true);
	Button_guns->SetVisibility(ESlateVisibility::Visible);
	Button_UpGun->SetIsEnabled(true);
	Button_UpGun->SetVisibility(ESlateVisibility::Hidden);
	Button_MidGun->SetIsEnabled(true);
	Button_MidGun->SetVisibility(ESlateVisibility::Hidden);
	Button_LowGun->SetIsEnabled(true);
	Button_LowGun->SetVisibility(ESlateVisibility::Hidden);
	Button_gun00->SetVisibility(ESlateVisibility::Hidden);
	Button_gun01->SetVisibility(ESlateVisibility::Hidden);
	Button_Delete0->SetVisibility(ESlateVisibility::Collapsed);
	Button_gun10->SetVisibility(ESlateVisibility::Hidden);
	Button_gun11->SetVisibility(ESlateVisibility::Hidden);
	Button_Delete1->SetVisibility(ESlateVisibility::Collapsed);
	Button_gun20->SetVisibility(ESlateVisibility::Hidden);
	Button_gun21->SetVisibility(ESlateVisibility::Hidden);
	Button_Delete2->SetVisibility(ESlateVisibility::Collapsed);

	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);
}



void UTowerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
}



void UTowerWidget::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);
}
