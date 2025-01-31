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
	BuildBoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_BoomTower.NI_BoomTower'")));

	//Destroy FX for lighters
	LightBoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight0.NI_BoomLight0'")));
	LightBoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_BoomLight1.NI_BoomLight1'")));

	//Destroy FX for projectile 0
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_sh.NI_Boom0_sh'")));
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_gr.NI_Boom0_gr'")));
	Proj0BoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom0_obj.NI_Boom0_obj'")));

	//Destroy FX for projectile 1
	Proj1BoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom1_gr.NI_Boom1_gr'")));
	Proj1BoomSystem.Add(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Boom1_obj.NI_Boom1_obj'")));

}



//Launch from an external actor
void ABoom::CreateBoomFunc(FVector Loc, FRotator Rot, UNiagaraSystem* Niagara, FColor Color)
{
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, RootComponent, NAME_None, Loc,
		Rot, EAttachLocation::Type::KeepRelativeOffset, true);
	NiagaraComponent->SetVariableLinearColor(FName(TEXT("Color")), Color);
}





// Called when the game starts or when spawned
void ABoom::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABoom::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
}

