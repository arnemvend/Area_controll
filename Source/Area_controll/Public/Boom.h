// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boom.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;
class UPrimitiveComponent;




UCLASS()
class AREA_CONTROLL_API ABoom : public AActor
{
	GENERATED_BODY()


public:	

	ABoom();



	UPROPERTY() UNiagaraComponent* NiagaraComponent;
	UPROPERTY() TArray<UPrimitiveComponent*> AimCompArr;

	UPROPERTY() TArray<UNiagaraComponent*> AimNComponents;

	UPROPERTY() UNiagaraSystem* AimNSystem;

	UPROPERTY() UNiagaraSystem* ProjT01BoomSystem;
	UPROPERTY() UNiagaraSystem* ProjT10BoomSystem;
	UPROPERTY() UNiagaraSystem* ProjT11BoomSystem;
	UPROPERTY() UNiagaraSystem* ProjT20BoomSystem;
	UPROPERTY() UNiagaraSystem* Proj2BoomSystem;

	UPROPERTY() TArray<UNiagaraSystem*> BuildBoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> Proj0BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> Proj1BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> ProjT00BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> ProjT22BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> ProjT23BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> LightBoomSystem;
	



	UFUNCTION() void CreateAimFunc(TArray<UPrimitiveComponent*> Components);
	UFUNCTION() void DeleteAimFunc();

	UFUNCTION() void CreateBoomFunc(FVector Loc, FRotator Rot, UNiagaraSystem* Niagara, FColor Color);

protected:

	
};
