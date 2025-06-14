// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameWidgetInterface.h"
#include "GroundActor.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UBoxComponent;


UCLASS()
class AREA_CONTROLL_API AGroundActor : public AActor, public IGameWidgetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroundActor();

	virtual void Tick(float DeltaTime) override;

	virtual FOnDeTouchDelegate& GetOnDeTouchDelegate() override;
	virtual FOnShieldDelegate& GetOnShieldDelegate() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* GroundMesh;
	UPROPERTY() UStaticMesh* GroundStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UMaterialInterface* GroundMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger") UBoxComponent* BoxComponent;


	UFUNCTION() void Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UPROPERTY() FOnDeTouchDelegate OnDeTouchDelegate;
	UPROPERTY() FOnShieldDelegate OnShieldDelegate;
};
