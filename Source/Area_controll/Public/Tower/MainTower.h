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

	AMainTower();

	virtual void BeginPlay() override;

	UPROPERTY() float UpBorder;
	UPROPERTY() float DownBorder;
	UPROPERTY() float RightBorder;
	UPROPERTY() float LeftBorder;
	UPROPERTY() int MassEnergy;
	UPROPERTY() int MaxEnergy;
	UPROPERTY() int NotNet;






	//"Logical functions for Energy"------------------------------------------------------------------------------------>
	UFUNCTION() void CheckEnergy();//Step of area calculating
	UFUNCTION() void CheckNew(ATower* Tower);
	UFUNCTION() void ReloadEnergy(FName DName);
	UFUNCTION() void MainFinder(ATower* TTower);
	UFUNCTION() void ReFinder(FName BName);
	UFUNCTION() void NetStart(FColor Color);


protected:

	
	UPROPERTY() int EnergyPoint;
	UPROPERTY() float EnergyStep;




};
