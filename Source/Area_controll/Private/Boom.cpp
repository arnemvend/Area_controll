// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"



// Sets default values
ABoom::ABoom()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	

	//Destroy FX for builds
	BuildBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_BoomTower.NI_BoomTower'")));
	BuildBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Construction.NI_Construction'")));
	BuildBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomWF.NI_BoomWF'")));

	//Destroy FX for lighters
	LightBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight0.NI_BoomLight0'")));
	LightBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight1.NI_BoomLight1'")));
	LightBoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight2.NI_BoomLight2'")));

	//Destroy FX for projectile 0
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_sh.NI_Boom0_sh'")));
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_gr.NI_Boom0_gr'")));
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_obj.NI_Boom0_obj'")));

	//Destroy FX for projectile 1
	Proj1BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom1_gr.NI_Boom1_gr'")));
	Proj1BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom1_obj.NI_Boom1_obj'")));

	//Destroy FX for projectile 2
	Proj2BoomSystem = LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom2_obj.NI_Boom2_obj'"));

	//Destroy FX for projectile T00
	ProjT00BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT00_gr.NI_BoomT00_gr'")));
	ProjT00BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT00_obj.NI_BoomT00_obj'")));

	//Destroy FX for projectile T01
	ProjT01BoomSystem = LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT01_obj.NI_BoomT01_obj'"));

	//Destroy FX for projectile T10
	ProjT10BoomSystem = LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT10_obj.NI_BoomT10_obj'"));

	//Destroy FX for projectile T20
	ProjT20BoomSystem = LoadObject<UNiagaraSystem>
	    (nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT20_gr.NI_BoomT20_gr'"));

	//Destroy FX for projectile T22
	ProjT22BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT22_gr.NI_BoomT22_gr'")));
	ProjT22BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT22_obj.NI_BoomT22_obj'")));

	//Destroy FX for projectile T23
	ProjT23BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT23_sh.NI_BoomT23_sh'")));
	ProjT23BoomSystem.Add(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomT23_obj.NI_BoomT23_obj'")));
}



//Launch from an external actor
void ABoom::CreateBoomFunc(FVector Loc, FRotator Rot, UNiagaraSystem* Niagara, FColor Color)
{
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, RootComponent, NAME_None, Loc,
		Rot, EAttachLocation::Type::KeepWorldPosition, true);
	NiagaraComponent->SetVariableLinearColor(FName(TEXT("Color")), Color);
}

