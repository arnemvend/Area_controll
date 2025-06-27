// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu_PlayerController.generated.h"


UCLASS()
class AREA_CONTROLL_API AMainMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:

	virtual void SetupInputComponent() override;
	
};
