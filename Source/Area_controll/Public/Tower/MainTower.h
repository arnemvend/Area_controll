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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float UpBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float DownBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float RightBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables") float LeftBorder;



	//"Logical functions for Energy"------------------------------------------------------------------------------------>
	UFUNCTION() void CheckEnergy();//Step of area calculating
	UFUNCTION() void CheckNew(ATower* Tower);
	UFUNCTION() void ReloadEnergy(FName DName);
	UFUNCTION() void MainFinder(ATower* TTower);
	UFUNCTION() void ReFinder(FName BName);




protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Variables") int MassEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") int EnergyPoint;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float EnergyStep;

};
