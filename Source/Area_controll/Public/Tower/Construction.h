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
class UNiagaraComponent;
class UNiagaraSystem;




UCLASS()
class AREA_CONTROLL_API AConstruction : public AActor
{
	GENERATED_BODY()
	
public:

	AConstruction();


	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") FColor Color;

	UFUNCTION() void ColorFunc();

	UPROPERTY() bool IsYour;

	

protected:
	
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* TowerMesh;

	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CapsuleInternal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* CapsuleExternal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* TowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInstanceDynamic* DTowMaterial;

	UPROPERTY() ATower* Tower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraBr;
	UPROPERTY() UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Max;
	UPROPERTY() float Health_P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float Step;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ATower> Spowned;

	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;
	


	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION() void AddHealthFunc();
	UFUNCTION() void HappyEnd();

	

};
