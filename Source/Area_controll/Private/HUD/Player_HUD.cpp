// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "HUD/Player_HUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/GameWidget.h"
#include "HUD/TowerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Tower/MainTower.h"


void APlayer_HUD::BeginPlay()
{
	Super::BeginPlay();

    //GameWidget create
    if (WidgetClass)
    {
        if (!IsValid(GameWidget))
        {
            GameWidget = CreateWidget<UGameWidget>(GetWorld(), WidgetClass);
        }
        if (IsValid(GameWidget))
        {
            GameWidget->AddToViewport();
        }
    }

    //TowerWidget create
    if (TWidgetClass)
    {
	    TWidget = CreateWidget<UTowerWidget>(GetWorld(), TWidgetClass);
        //the object has not been added to the screen, the link is passed to the MainTower
        AMainTower* Main;
        TArray<AActor*> MTowers;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainTower::StaticClass(), MTowers);
        if (MTowers.Num() != 0 && IsValid(TWidget))
        {
	        for (int i = 0; i < MTowers.Num(); i++)
	        {
                Main = Cast<AMainTower>(MTowers[i]);
                if (IsValid(Main) && Main->ActorHasTag(TEXT("Main")))
                {
                    Main->TW = TWidget;
                }
	        }
        }
        Main = nullptr;
        MTowers.Empty(0);
    }

}
