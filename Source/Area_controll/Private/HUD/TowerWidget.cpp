// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/TowerWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Tower/Tower.h"



bool UTowerWidget::Initialize()
{
	Super::Initialize();

	Button_shield->OnClicked.AddDynamic(this, &UTowerWidget::Button_shieldClick);
	Button_destroy->OnClicked.AddDynamic(this, &UTowerWidget::Button_destroyClick);
	Button_Repeater->OnClicked.AddDynamic(this, &UTowerWidget::Button_RepeaterClick);

	return true;
}



void UTowerWidget::StartEvent()
{
	if (MyTower && MyTower->ActorHasTag(TEXT("Main")))
	{
		Button_Repeater->SetIsEnabled(false);
		Button_Repeater->SetVisibility(ESlateVisibility::Collapsed);
		Button_destroy->SetIsEnabled(false);
		Button_destroy->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (MyTower && MyTower->IsRepeater)
	{
		Button_Repeater->SetIsEnabled(false);
		Button_Repeater->SetVisibility(ESlateVisibility::Collapsed);
		Button_shield->SetIsEnabled(false);
		Button_shield->SetVisibility(ESlateVisibility::Collapsed);
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
		MyTower->DeTouch();
	}
}


//destroy the tower
void UTowerWidget::Button_destroyClick()
{
	if (MyTower)
	{
		MyTower->TowerDestroy();
	}
}


//enables/disables shield
void UTowerWidget::Button_shieldClick()
{
	if (MyTower)
	{
		MyTower->ShieldCreate();
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
}

