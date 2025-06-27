// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu_GameMode.generated.h"



class UMainMenu;


UCLASS()
class AREA_CONTROLL_API AMainMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:

	virtual void BeginPlay() override;

	UPROPERTY() UMainMenu* MainMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") TSubclassOf<UMainMenu> MainMenuClass;
	
};
