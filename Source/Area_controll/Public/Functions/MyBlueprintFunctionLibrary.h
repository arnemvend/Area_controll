// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"


UCLASS()
class AREA_CONTROLL_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Custom")
	static void TouchToWorld(float X, float Y, TArray<AActor*> Actors, FVector2D& Position, APlayerController* PlayerController, UObject* WorldContextObject);

};
