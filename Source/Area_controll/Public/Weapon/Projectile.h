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
	
	AProjectile();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY() AActor* Gun;
	UPROPERTY() UPrimitiveComponent* Component;
	UPROPERTY() FVector Aim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aims") TArray<FName> EnemyNames;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	


};
