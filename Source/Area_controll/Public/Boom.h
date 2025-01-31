// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boom.generated.h"


class UNiagaraSystem;
class USceneComponent;
class UNiagaraComponent;



UCLASS()
class AREA_CONTROLL_API ABoom : public AActor
{
	GENERATED_BODY()


public:	

	ABoom();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY() UNiagaraComponent* NiagaraComponent;

	UPROPERTY() TArray<UNiagaraSystem*> BuildBoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> Proj0BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> Proj1BoomSystem;
	UPROPERTY() TArray<UNiagaraSystem*> LightBoomSystem;



	UPROPERTY() float Duration;

	UFUNCTION() void CreateBoomFunc(FVector Loc, FRotator Rot, UNiagaraSystem* Niagara, FColor Color);

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY() FTimerHandle Timer0;

	
};
