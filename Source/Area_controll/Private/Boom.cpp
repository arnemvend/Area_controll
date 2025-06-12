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
}



//Launch from an external actor
void ABoom::CreateBoomFunc(FVector Loc, FRotator Rot, UNiagaraSystem* Niagara, FColor Color)
{
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, RootComponent, NAME_None, Loc,
		Rot, EAttachLocation::Type::KeepRelativeOffset, true);
	NiagaraComponent->SetVariableLinearColor(FName(TEXT("Color")), Color);
}

