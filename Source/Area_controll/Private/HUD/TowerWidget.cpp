// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/TowerWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Tower/AreaRadius.h"
#include "Tower/Tower.h"
//#include "UObject/FastReferenceCollector.h"


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
	Button_gun02->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun02Click);
	Button_gun03->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun03Click);
	Button_gun10->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun10Click);
	Button_gun11->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun11Click);
	Button_gun12->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun12Click);
	Button_gun13->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun13Click);
	Button_gun20->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun20Click);
	Button_gun21->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun21Click);
	Button_gun22->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun22Click);
	Button_gun23->OnClicked.AddDynamic(this, &UTowerWidget::Button_gun23Click);
	Button_Delete0->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete0Click);
	Button_Delete1->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete1Click);
	Button_Delete2->OnClicked.AddDynamic(this, &UTowerWidget::Button_Delete2Click);
	CheckBox_Auto->OnCheckStateChanged.AddDynamic(this, &UTowerWidget::CheckAutoOnChecked);
	Button_Aim->OnClicked.AddDynamic(this, &UTowerWidget::Button_AimClick);
	Button_modify->OnClicked.AddDynamic(this, &UTowerWidget::Button_modifyClick);

	IsFirst = true;

	return true;
}






void UTowerWidget::StartEvent()
{
	if (!IsValid(MyTower))
	{
		return;
	}
	if (MyTower->ActorHasTag(TEXT("Main")))
	{
		Button_Repeater->SetVisibility(ESlateVisibility::Hidden);
		Button_destroy->SetVisibility(ESlateVisibility::Hidden);
		Button_guns->SetVisibility(ESlateVisibility::Hidden);
		Button_modify->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (MyTower->IsRepeater)
	{
		Button_Repeater->SetVisibility(ESlateVisibility::Hidden);
		Button_shield->SetVisibility(ESlateVisibility::Hidden);
		Button_guns->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (MyTower->GunNum > 0)
	{
		Button_Repeater->SetVisibility(ESlateVisibility::Hidden);
		if (MyTower->HaveLowGun)
		{
			CheckBox_Auto->SetIsChecked(MyTower->IsAuto);
			CheckBox_Auto->SetVisibility(ESlateVisibility::Visible);
			if (!MyTower->IsAuto)
			{
				Button_Aim->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else 
	{
		if (!Button_Repeater->IsVisible())
		{
			Button_Repeater->SetVisibility(ESlateVisibility::Visible);
		}
		CheckBox_Auto->SetVisibility(ESlateVisibility::Hidden);
		Button_Aim->SetVisibility(ESlateVisibility::Hidden);
	}
}



void UTowerWidget::MoveStyle()
{
	if (ActiveButton)
	{
		ButtonStyle = ActiveButton->GetStyle();
		ButtonStyle.Normal = ButtonStyle.Disabled;
		ButtonStyle.Disabled = ButtonStyle.Hovered;
		ButtonStyle.Hovered = ButtonStyle.Normal;
		ActiveButton->SetStyle(ButtonStyle);
	}
	if (ActiveGunButton)
	{
		ButtonStyle = ActiveGunButton->GetStyle();
		ButtonStyle.Normal = ButtonStyle.Pressed;
		ButtonStyle.Pressed = ButtonStyle.Hovered;
		ButtonStyle.Hovered = ButtonStyle.Normal;
		ActiveGunButton->SetStyle(ButtonStyle);
	}
}


void UTowerWidget::MovePriceStyle(UButton*& Button)
{
	ButtonPriceStyle = Button->GetStyle();
	ButtonPriceStyle.Normal = ButtonPriceStyle.Disabled;
	ButtonPriceStyle.Disabled = ButtonPriceStyle.Hovered;
	ButtonPriceStyle.Hovered = ButtonPriceStyle.Normal;
	Button->SetStyle(ButtonPriceStyle);
}


void UTowerWidget::CloseGuns()
{
	HorizontalBox_UpGuns->SetVisibility(ESlateVisibility::Collapsed);
	HorizontalBox_MidGuns->SetVisibility(ESlateVisibility::Collapsed);
	HorizontalBox_LowGuns->SetVisibility(ESlateVisibility::Collapsed);

	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);

	if (IsValid(AreaRadius))
	{
		AreaRadius->Stop();
	}
}


bool UTowerWidget::PriceUpdate(bool IsGood, UButton*& Button)
{
	bool bExit = false;
	if (IsGood)
	{
		if (UnActiveButtons.Contains(Button))
		{
			UnActiveButtons.RemoveSwap(Button);
			MovePriceStyle(Button);
			bExit = true;
		}
	}
	else
	{
		if (!UnActiveButtons.Contains(Button))
		{
			if (ActiveGunButton == Button)
			{
				MoveStyle();
				ActiveGunButton = nullptr;
			}
			UnActiveButtons.AddUnique(Button);
			MovePriceStyle(Button);
			bExit = true;
		}
	}
	return bExit;
}


void UTowerWidget::ButtonGunFunc(UButton*& Button, UChildActorComponent*& Component, int Type, int Number,
                                 UHorizontalBox*& Box, UImage*& Image, FTimerHandle& Timer)
{
	if (ActiveGunButton == Button)
	{
		const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(Component) && !IsValid(Component->GetChildActor());

		if (bHasActiveGun)
		{
			MyTower->CreateGun(Type, Number);
		}
		if (IsValid(AreaRadius))
		{
			AreaRadius->Stop();
		}
		switch (Type)
		{
			case 0: AdressUpGun = Number; break;
			case 1: AdressMidGun = 10 + Number; break;
			case 2: AdressLowGun = 20 + Number; break;
			default: return;
		}

		FButtonStyle Style = Button->GetStyle();
		Image->SetBrush(Style.Pressed);
		
		Box->SetVisibility(ESlateVisibility::Collapsed);
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}
	else
	{
		MoveStyle();
		ActiveGunButton = Button;
		MoveStyle();
		
		if (IsValid(AreaRadius) && IsValid(MyTower) && IsValid(GInstance))
		{
			AreaRadius->SetRadius(GInstance->GunProperty(Type, Number, "Radius"), 
				GInstance->GunProperty(Type, Number, "ExtRadius"),
				MyTower->GetActorLocation());
		}
	}
}


void UTowerWidget::DeleteGunFunc(UButton*& Button, UChildActorComponent*& Component, int Type, UHorizontalBox*& Box)
{
	if (ActiveButton == Button)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;

		const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(Component) && IsValid(Component->GetChildActor());
		if (bHasActiveGun)
		{
			MyTower->DeleteGun(Type);
			Box->SetVisibility(ESlateVisibility::Collapsed);
			VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(AreaRadius))
		{
			AreaRadius->Stop();
		}
	}
	else
	{
		MoveStyle();
		ActiveButton = Button;
		MoveStyle();
	}
}


//enables repeater mode for the tower
void UTowerWidget::Button_RepeaterClick()
{
	if (ActiveButton == Button_Repeater && IsValid(MyTower))
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
		MyTower->Repeater();
		Button_shield->SetVisibility(ESlateVisibility::Hidden);
		Button_Repeater->SetVisibility(ESlateVisibility::Hidden);
		VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
		MyTower->DeTouch(0);
	}
	else if (IsValid(MyTower))
	{
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		CloseGuns();
		MoveStyle();
		ActiveButton = Button_Repeater;
		MoveStyle();
		MyTower->UnSetAimComponents();
	}
}


//destroy the tower
void UTowerWidget::Button_destroyClick()
{
	if (ActiveButton == Button_destroy && IsValid(MyTower))
	{
		MoveStyle();
		VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
		MyTower->RunDelete();
	}
	else if (IsValid(MyTower))
	{
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		CloseGuns();
		MoveStyle();
		ActiveButton = Button_destroy;
		MoveStyle();
		MyTower->UnSetAimComponents();
	}
}


//enables/disables shield
void UTowerWidget::Button_shieldClick()
{
	VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
	VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
	CloseGuns();
	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}
	if (IsValid(MyTower))
	{
		MyTower->ShieldCreate();
		MyTower->UnSetAimComponents();
	}
}





void UTowerWidget::Button_modifyClick()
{
	if (VerticalBox_Mod->IsVisible())
	{
		VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		VerticalBox_Mod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	CloseGuns();
	VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}
}




//TowerGuns menu enables/disables
void UTowerWidget::Button_gunsClick()
{
	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}
	VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
	if (VerticalBox_Guns->IsVisible())
	{
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		CloseGuns();
	}
	else
	{
		VerticalBox_Guns->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (IsValid(MyTower))
	{
		MyTower->UnSetAimComponents();
	}
}




//Gun of 0 type menu
void UTowerWidget::Button_UpGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->UpGun) &&
		IsValid(MyTower->UpGun->GetChildActor());

	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}

	if (!HorizontalBox_UpGuns->IsVisible())
	{
		Button_gun00->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun01->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun02->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun03->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_Delete0->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Image_Up->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		HorizontalBox_MidGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_LowGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_UpGuns->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HorizontalBox_UpGuns->InvalidateLayoutAndVolatility();

		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);

		if(bHasActiveGun)
		{
			if (IsValid(AreaRadius) && IsValid(MyTower) && IsValid(GInstance))
			{
				AreaRadius->SetRadius(GInstance->GunProperty(0, AdressUpGun, "Radius"), 
				GInstance->GunProperty(0, AdressUpGun, "ExtRadius"),
				MyTower->GetActorLocation());
			}
		}
		else
		{
			if (IsValid(AreaRadius))
			{
				AreaRadius->Stop();
			}

			GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
			{
				if (IsValid(GInstance) && IsValid(GMode))
				{
					bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG00_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun00))
					{
						if (bGoodPrice)
						{
							Button_gun00->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun00Click);
						}
						else
						{
							Button_gun00->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun00Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG01_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun01))
					{
						if (bGoodPrice)
						{
							Button_gun01->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun01Click);
						}
						else
						{
							Button_gun01->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun01Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG02_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun02))
					{
						if (bGoodPrice)
						{
							Button_gun02->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun02Click);
						}
						else
						{
							Button_gun02->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun02Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG03_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun03))
					{
						if (bGoodPrice)
						{
							Button_gun03->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun03Click);
						}
						else
						{
							Button_gun03->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun03Click);
						}
					}
						
				}
			}, 0.2f, true, 0.0f);	
		}
	}
	else
	{
		HorizontalBox_UpGuns->SetVisibility(ESlateVisibility::Collapsed);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);

		if (IsValid(AreaRadius))
		{
			AreaRadius->Stop();
		}
	}
	
}



//Gun of 1 type menu
void UTowerWidget::Button_MidGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->MidGun) &&
		IsValid(MyTower->MidGun->GetChildActor());

	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}

	if (!HorizontalBox_MidGuns->IsVisible())
	{
		Button_gun10->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun11->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun12->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun13->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_Delete1->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Image_Mid->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		HorizontalBox_UpGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_LowGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_MidGuns->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HorizontalBox_MidGuns->InvalidateLayoutAndVolatility();

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);

		if (bHasActiveGun)
		{
			if (IsValid(AreaRadius) && IsValid(MyTower) && IsValid(GInstance))
			{
				AreaRadius->SetRadius(GInstance->GunProperty(1, AdressMidGun % 10, "Radius"), 
				GInstance->GunProperty(1, AdressMidGun % 10, "ExtRadius"),
				MyTower->GetActorLocation());
			}
		}
		else
		{
			if (IsValid(AreaRadius))
			{
				AreaRadius->Stop();
			}

			GetWorld()->GetTimerManager().SetTimer(Timer1, [this]()
			{
				if (IsValid(GInstance) && IsValid(GMode))
				{
					bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG10_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun10))
					{
						if (bGoodPrice)
						{
							Button_gun10->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun10Click);
						}
						else
						{
							Button_gun10->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun10Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG11_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun11))
					{
						if (bGoodPrice)
						{
							Button_gun11->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun11Click);
						}
						else
						{
							Button_gun11->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun11Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG12_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun12))
					{
						if (bGoodPrice)
						{
								Button_gun12->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun12Click);
						}
						else
						{
							Button_gun12->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun12Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG13_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun13))
					{
						if (bGoodPrice)
						{
							Button_gun13->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun13Click);
						}
						else
						{
							Button_gun13->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun13Click);
						}
					}
				}
			}, 0.2f, true, 0.0f);
		}
	}
	else
	{
		HorizontalBox_MidGuns->SetVisibility(ESlateVisibility::Collapsed);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);

		if (IsValid(AreaRadius))
		{
			AreaRadius->Stop();
		}
	}
}



//Gun of 2 type menu
void UTowerWidget::Button_LowGunClick()
{
	const bool bHasActiveGun = IsValid(MyTower) &&
		IsValid(MyTower->LowGun) &&
		IsValid(MyTower->LowGun->GetChildActor());

	if (ActiveButton)
	{
		MoveStyle();
		ActiveButton = nullptr;
		ActiveGunButton = nullptr;
	}

	if (!HorizontalBox_LowGuns->IsVisible())
	{
		Button_gun20->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun21->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun22->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_gun23->SetVisibility(bHasActiveGun ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		Button_Delete2->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Image_Low->SetVisibility(bHasActiveGun ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		HorizontalBox_UpGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_MidGuns->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_LowGuns->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HorizontalBox_LowGuns->InvalidateLayoutAndVolatility();

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);

		if (bHasActiveGun)
		{
			if (IsValid(AreaRadius) && IsValid(MyTower) && IsValid(GInstance))
			{
				AreaRadius->SetRadius(GInstance->GunProperty(2, AdressLowGun % 10, "Radius"), 
				GInstance->GunProperty(2, AdressLowGun % 10, "ExtRadius"),
				MyTower->GetActorLocation());
			}
		}
		else
		{
			if (IsValid(AreaRadius))
			{
				AreaRadius->Stop();
			}

			GetWorld()->GetTimerManager().SetTimer(Timer2, [this]()
			{
				if (IsValid(GInstance) && IsValid(GMode))
				{
					bool bGoodPrice = GMode->PlayerEnergy >= GInstance->TG20_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun20))
					{
						if (bGoodPrice)
						{
							Button_gun20->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun20Click);
						}
						else
						{
							Button_gun20->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun20Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG21_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun21))
					{
						if (bGoodPrice)
						{
							Button_gun21->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun21Click);
						}
						else
						{
							Button_gun21->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun21Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG22_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun22))
					{
						if (bGoodPrice)
						{
							Button_gun22->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun22Click);
						}
						else
						{
							Button_gun22->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun22Click);
						}
					}

					bGoodPrice = GMode->PlayerEnergy >= GInstance->TG23_EnergyPrice;
					if (PriceUpdate(bGoodPrice, Button_gun23))
					{
						if (bGoodPrice)
						{
							Button_gun23->OnClicked.AddUniqueDynamic(this, &UTowerWidget::Button_gun23Click);
						}
						else
						{
							Button_gun23->OnClicked.RemoveDynamic(this, &UTowerWidget::Button_gun23Click);
						}
					}
				}
			}, 0.2f, true, 0.0f);
		}
	}
	else
	{
		HorizontalBox_LowGuns->SetVisibility(ESlateVisibility::Collapsed);

		GetWorld()->GetTimerManager().ClearTimer(Timer0);
		GetWorld()->GetTimerManager().ClearTimer(Timer1);
		GetWorld()->GetTimerManager().ClearTimer(Timer2);

		if (IsValid(AreaRadius))
		{
			AreaRadius->Stop();
		}
	}
}



void UTowerWidget::Button_gun00Click()
{
	ButtonGunFunc(Button_gun00, MyTower->UpGun, 0, 0, HorizontalBox_UpGuns, Image_Up, Timer0);
}


void UTowerWidget::Button_gun01Click()
{
	ButtonGunFunc(Button_gun01, MyTower->UpGun, 0, 1, HorizontalBox_UpGuns, Image_Up, Timer0);
}


void UTowerWidget::Button_gun02Click()
{
	ButtonGunFunc(Button_gun02, MyTower->UpGun, 0, 2, HorizontalBox_UpGuns, Image_Up, Timer0);
}


void UTowerWidget::Button_gun03Click()
{
	ButtonGunFunc(Button_gun03, MyTower->UpGun, 0, 3, HorizontalBox_UpGuns, Image_Up, Timer0);
}


void UTowerWidget::Button_gun10Click()
{
	ButtonGunFunc(Button_gun10, MyTower->MidGun, 1, 0, HorizontalBox_MidGuns, Image_Mid, Timer1);
}


void UTowerWidget::Button_gun11Click()
{
	ButtonGunFunc(Button_gun11, MyTower->MidGun, 1, 1, HorizontalBox_MidGuns,Image_Mid, Timer1);
}


void UTowerWidget::Button_gun12Click()
{
	ButtonGunFunc(Button_gun12, MyTower->MidGun, 1, 2, HorizontalBox_MidGuns,Image_Mid, Timer1);
}


void UTowerWidget::Button_gun13Click()
{
	ButtonGunFunc(Button_gun13, MyTower->MidGun, 1, 3, HorizontalBox_MidGuns,Image_Mid, Timer1);
}


void UTowerWidget::Button_gun20Click()
{
	ButtonGunFunc(Button_gun20, MyTower->LowGun, 2, 0, HorizontalBox_LowGuns,Image_Low, Timer2);
}


void UTowerWidget::Button_gun21Click()
{
	ButtonGunFunc(Button_gun21, MyTower->LowGun, 2, 1, HorizontalBox_LowGuns,Image_Low, Timer2);
}


void UTowerWidget::Button_gun22Click()
{
	ButtonGunFunc(Button_gun22, MyTower->LowGun, 2, 2, HorizontalBox_LowGuns,Image_Low, Timer2);
}


void UTowerWidget::Button_gun23Click()
{
	ButtonGunFunc(Button_gun23, MyTower->LowGun, 2, 3, HorizontalBox_LowGuns,Image_Low, Timer2);
}


void UTowerWidget::Button_Delete0Click()
{
	DeleteGunFunc(Button_Delete0, MyTower->UpGun, 0, HorizontalBox_UpGuns);
}


void UTowerWidget::Button_Delete1Click()
{
	DeleteGunFunc(Button_Delete1, MyTower->MidGun, 1, HorizontalBox_MidGuns);
}


void UTowerWidget::Button_Delete2Click()
{
	DeleteGunFunc(Button_Delete2, MyTower->LowGun, 2, HorizontalBox_LowGuns);
}


void UTowerWidget::CheckAutoOnChecked(bool bCheck)
{
	if (IsValid(MyTower))
	{
		MyTower->SetAutoFire(bCheck);
		MyTower->UnSetAimComponents();
		MyTower->OnOffTargetEffect(false);
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
		CloseGuns();
	}
	Button_Aim->SetVisibility(bCheck ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}


void UTowerWidget::Button_AimClick()
{
	if (IsValid(MyTower) && MyTower->HaveLowGun)
	{
		MyTower->SetAimComponents();
		VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
		VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
		CloseGuns();
	}
}



void UTowerWidget::Reset()
{
	Button_Repeater->SetVisibility(ESlateVisibility::Visible);
	Button_destroy->SetVisibility(ESlateVisibility::Visible);
	Button_shield->SetVisibility(ESlateVisibility::Visible);
	Button_guns->SetVisibility(ESlateVisibility::Visible);
	Button_modify->SetVisibility(ESlateVisibility::Visible);
	VerticalBox_Guns->SetVisibility(ESlateVisibility::Collapsed);
	VerticalBox_Mod->SetVisibility(ESlateVisibility::Collapsed);
	CheckBox_Auto->SetVisibility(ESlateVisibility::Hidden);
	Button_Aim->SetVisibility(ESlateVisibility::Hidden);
	CloseGuns();

	MoveStyle();
	ActiveButton = nullptr;
	ActiveGunButton = nullptr;
}



void UTowerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsFirst)
	{
		return;
	}

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
	AreaRadius = Cast<AAreaRadius>(UGameplayStatics::GetActorOfClass(GetWorld(), AAreaRadius::StaticClass()));

	if (IsValid(GInstance))
	{
		TextBlock_gun00->SetText(FText::AsNumber(GInstance->TG00_EnergyPrice));
		TextBlock_gun01->SetText(FText::AsNumber(GInstance->TG01_EnergyPrice));
		TextBlock_gun02->SetText(FText::AsNumber(GInstance->TG02_EnergyPrice));
		TextBlock_gun03->SetText(FText::AsNumber(GInstance->TG03_EnergyPrice));
		TextBlock_gun10->SetText(FText::AsNumber(GInstance->TG10_EnergyPrice));
		TextBlock_gun11->SetText(FText::AsNumber(GInstance->TG11_EnergyPrice));
		TextBlock_gun12->SetText(FText::AsNumber(GInstance->TG12_EnergyPrice));
		TextBlock_gun13->SetText(FText::AsNumber(GInstance->TG13_EnergyPrice));
		TextBlock_gun20->SetText(FText::AsNumber(GInstance->TG20_EnergyPrice));
		TextBlock_gun21->SetText(FText::AsNumber(GInstance->TG21_EnergyPrice));
		TextBlock_gun22->SetText(FText::AsNumber(GInstance->TG22_EnergyPrice));
		TextBlock_gun23->SetText(FText::AsNumber(GInstance->TG23_EnergyPrice));
	}

	IsFirst = false;
}




void UTowerWidget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);

	if (IsValid(AreaRadius))
	{
		AreaRadius->Stop();
	}

	Super::NativeDestruct();
}
