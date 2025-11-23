// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/AreaControll_GameMode.h"




AAreaControll_GameMode::AAreaControll_GameMode()
{
	RepeaterMesh = LoadObject<UStaticMesh>
	(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Repeater.Repeater'"));

	PlayerEnergy = 0;
	EnemyEnergy = 0;
	PlayerMaxEnergy = 0;
	EnemyMaxEnergy = 0;
}
