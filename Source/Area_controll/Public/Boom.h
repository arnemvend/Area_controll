// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boom.generated.h"


class UNiagaraComponent;
class UNiagaraSystem;



UCLASS()
class AREA_CONTROLL_API ABoom : public AActor
{
	GENERATED_BODY()


public:	

	ABoom();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara") UNiagaraComponent* NiagaraBoom;
	UPROPERTY() UNiagaraSystem* NiagaraBoomSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Duration;

	UFUNCTION() void Boom();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;


	UPROPERTY() FTimerHandle Timer0;

	
};
