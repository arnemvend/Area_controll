// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile_02.h"
#include "Core/AreaControll_GameInstance.h"




AtProjectile_02::AtProjectile_02()
{
	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
}





void AtProjectile_02::SetterFunc()
{
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG02_Damage;
	}
}
