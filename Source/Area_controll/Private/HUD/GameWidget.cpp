// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/GameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Core/AreaControll_PlayerController.h"
#include "Creator/BuildCreator.h"
#include "Kismet/GameplayStatics.h"
#include "Map/GroundActor.h"
#include "Tower/MainTower.h"




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
	if (IsValid(BCreator) == false)
	{
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FActorSpawnParameters SpawnParams;
		const FVector SpawnLoc = FVector(0.0f, 0.0f, -2000.0f);
		BCreator = GetWorld()->SpawnActor<ABuildCreator>(Spowned, SpawnLoc, SpawnRotation, SpawnParams);
		P_Controller->BuildCreator = BCreator;
		//reload ignored actors in Player_Controller
		P_Controller->OutActors.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), P_Controller->OutActors);
		P_Controller->OutActors.Remove(P_Controller->GroundActor);
		//bugfix. check. is there BuildCreator in world
		GetWorld()->GetTimerManager().SetTimer(Timer1, [this]()
			{
				if (IsValid(BCreator) == false)
				{
					P_Controller->CreatorIsHere = false;
					GetWorld()->GetTimerManager().ClearTimer(Timer1);
				}
			}, 0.04f, true);
		P_Controller->CreatorIsHere = true;
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
		}
		else
		{
			ShieldIsVisible = false;
		}
		TArray<AActor*> Towers;
		ATower* Tower;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
		if (Towers.Num() > 0)
		{
			for (int i = 0; i < Towers.Num(); i++)
			{
				if (Towers[i])
				{
					Tower = Cast<ATower>(Towers[i]);
					if(Tower->ActorHasTag(TEXT("Your"))
						&& Tower->SphereShieldMesh->IsVisible() == ShieldIsVisible
						&& Tower->IsProcessRun == false)
					{
						Tower->ShieldCreate();
					}
				}
			}
			//delay before reuse Button_AllShield
			GetWorld()->GetTimerManager().SetTimer(Timer2, [this]()
			{
				if (IsValid(BCreator) == false)
				{
					ShieldIsActive = !ShieldIsActive;
					ButtonTowerIsActive = true;
				}
			}, 1.1f, false);
		}
		Towers.Empty();
		Tower = nullptr;
	}
}


//if player use main menu, tower menu will off
void UGameWidget::DeactivatedTowerMenu()
{
	TArray<AActor*> Towers;
	ATower* Tower;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	if (Towers.Num() > 0)
	{
		for (int i = 0; i < Towers.Num(); i++)
		{
			if (Towers[i])
			{
				Tower = Cast<ATower>(Towers[i]);
				if (Tower->IsClicked)
				{
					Tower->DeTouch();
				}
			}
		}
	}
	Towers.Empty();
	Tower = nullptr;
}







void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	P_Controller = Cast<AAreaControll_PlayerController>
	(UGameplayStatics::GetActorOfClass(GetWorld(), AAreaControll_PlayerController::StaticClass()));
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainTower::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		for (int i = 0; i < Actors.Num(); i++)
		{
			if (Actors[i] && Actors[i]->ActorHasTag(TEXT("Main")))
			{
				Main = Cast<AMainTower>(Actors[i]);
			}
			if (Actors[i] && Actors[i]->ActorHasTag(TEXT("MainEnemy")))
			{
				MainEnemy = Cast<AMainTower>(Actors[i]);
			}
		}
	}
	Actors.Empty();

	GetWorld()->GetTimerManager().SetTimer(Timer0, [this]()
	{
		Text_FPS->SetText(FText::AsNumber(FMath::TruncToInt(1 / DeltaTime)));
		if (MainEnemy)
		{
			Text_EnemyEnergy->SetText(FText::AsNumber(MainEnemy->MassEnergy));
		}
		if (Main)
		{
			Text_YourEnergy->SetText(FText::AsNumber(Main->MassEnergy));
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
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(Timer0);
	GetWorld()->GetTimerManager().ClearTimer(Timer1);
	GetWorld()->GetTimerManager().ClearTimer(Timer2);
}




