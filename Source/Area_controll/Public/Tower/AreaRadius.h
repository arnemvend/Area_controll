// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaRadius.generated.h"


class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;




UCLASS()
class AREA_CONTROLL_API AAreaRadius : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAreaRadius();

	UFUNCTION() void SetRadius(float R1, float R2, FVector Loc);
	UFUNCTION() void Stop();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* AreaMesh;

	UPROPERTY() UMaterialInterface* AreaMaterial;

	UPROPERTY() UMaterialInstanceDynamic* DAreaMaterial;

	UPROPERTY() float StartNumber;

};
