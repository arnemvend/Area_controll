// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/MainMenu_GameMode.h"
#include "HUD/MainMenu.h"



void AMainMenu_GameMode::BeginPlay()
{
	Super::BeginPlay();

    //GameWidget create
    if (MainMenuClass)
    {
        if (!IsValid(MainMenu))
        {
            MainMenu = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
        }
        if (IsValid(MainMenu))
        {
            MainMenu->AddToViewport();
        }
    }
}
