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
	// Sets default values for this actor's properties
	ABoom();
	virtual void Tick(float DeltaTime) override;

	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraBoom;
	UPROPERTY() UNiagaraSystem* NiagaraBoomSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Duration;

	UFUNCTION(BlueprintCallable) void Boom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


	UPROPERTY() FTimerHandle TimerHandle;

	
};
