// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Construction.generated.h"


class UStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;
class UCapsuleComponent;
class ATower;




UCLASS()
class AREA_CONTROLL_API AConstruction : public AActor
{
	GENERATED_BODY()
	
public:

	AConstruction();


	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") FColor Color;

	UFUNCTION() void ColorFunc();

	

	

protected:
	
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* RotMesh;

	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CapsuleInternal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* TowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* RotMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DTowMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DRotMaterial;

	UPROPERTY() ATower* Tower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") float Max;
	UPROPERTY() float Health_P;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") float Step;
	


	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION() void AddHealthFunc();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ATower> Spowned;

	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;

};
