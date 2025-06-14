// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"



class UPrimitiveComponent;


UCLASS()
class AREA_CONTROLL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY() AActor* Gun;
	UPROPERTY() UPrimitiveComponent* Component;
	UPROPERTY() FVector Aim;
	UPROPERTY() TArray<FName> EnemyNames;


	//UFUNCTION() virtual void SetBeginOvelap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


};
