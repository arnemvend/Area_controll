// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameWidgetInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeTouchDelegate, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldDelegate, bool, IsPlayer, bool, IsShieldOn);


UINTERFACE(MinimalAPI)
class UGameWidgetInterface : public UInterface
{
	GENERATED_BODY()
};



class AREA_CONTROLL_API IGameWidgetInterface
{
	GENERATED_BODY()

	
public:

	UFUNCTION() virtual FOnDeTouchDelegate& GetOnDeTouchDelegate() = 0;
	UFUNCTION() virtual FOnShieldDelegate& GetOnShieldDelegate() = 0;

};
