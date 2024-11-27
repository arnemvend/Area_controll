// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tower/Tower.h"
#include "MainTower.generated.h"


UCLASS()
class AREA_CONTROLL_API AMainTower : public ATower
{
	GENERATED_BODY()


public:

	UPROPERTY() float UpBorder;
	UPROPERTY() float DownBorder;
	UPROPERTY() float RightBorder;
	UPROPERTY() float LeftBorder;
	UPROPERTY() int MassEnergy;



	//"Logical functions for Energy"------------------------------------------------------------------------------------>
	UFUNCTION() void CheckEnergy();//Step of area calculating
	UFUNCTION() void CheckNew(ATower* Tower);
	UFUNCTION() void ReloadEnergy(FName DName);
	UFUNCTION() void MainFinder(ATower* TTower);
	UFUNCTION() void ReFinder(FName BName);




protected:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") int EnergyPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float EnergyStep;

};
