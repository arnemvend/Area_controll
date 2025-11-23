// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildFabric.generated.h"


class UStaticMeshComponent;
class UCapsuleComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UNiagaraComponent;
class UNiagaraSystem;
class UAreaControll_GameInstance;
class AWild;
class ABoom;



UCLASS()
class AREA_CONTROLL_API AWildFabric : public AActor
{
	GENERATED_BODY()
	
public:	

	AWildFabric();

	
	virtual void Tick(float DeltaTime) override;

	//in Level_BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") int Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") FVector AimCoord;

	


protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* FabricMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* SquareMesh;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY() UNiagaraSystem* NiagaraNetSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger") UCapsuleComponent* CaplsuleComponent;

	UPROPERTY() UMaterialInterface* Material;
	UPROPERTY() UMaterialInstanceDynamic* DMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TArray<TSubclassOf<AWild>> Spowned;

	UPROPERTY() UAreaControll_GameInstance* GInstance;

	UPROPERTY() ABoom* BoomActor;
	UPROPERTY() AWild* Wild;
	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY() bool CanDamage;


	UPROPERTY() float Health;
	UPROPERTY() float MaxHealth;

	UFUNCTION(BlueprintCallable) void RotFunc();//in LevelBP
	UFUNCTION(BlueprintCallable) void CreateFunc();//in LevelBP
	UFUNCTION() void SetDamageFunc();
	UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
};
