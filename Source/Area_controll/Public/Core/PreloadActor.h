// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreloadActor.generated.h"

class UNiagaraSystem;
class UStaticMesh;
class UMaterialInterface;
class UStaticMesh;

UCLASS()
class AREA_CONTROLL_API APreloadActor : public AActor
{
	GENERATED_BODY()
	
public:	

	APreloadActor();

	virtual void Tick(float DeltaTime) override;


	//Repeater--------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Repeater") UStaticMesh* RMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Repeater") UNiagaraSystem* RNSystem;


	//Colors----------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor DefaultColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor YourColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor EnemyColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color") FColor WildColor;



	//For Units-------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Units") TArray<float> Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Units") TArray<float> Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Units") TArray<float> High;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Units") TArray<UStaticMesh*> SM_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Units") TArray<UMaterialInterface*> MI_Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<UStaticMesh*> Gun_SM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<UMaterialInterface*> Gun_MI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<UNiagaraSystem*> Gun_NS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<float> Gun_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<float> Gun_Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<float> Gun_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wild Unit's guns") TArray<TSubclassOf<AActor>> Gun_BP;


protected:



	virtual void BeginPlay() override;


};
