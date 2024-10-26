// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructWild.generated.h"

class UNiagaraSystem;
class UStaticMesh;
class UMaterialInterface;


USTRUCT(BlueprintType)

struct AREA_CONTROLL_API FStructWild
{
	GENERATED_BODY()


public:

	FStructWild();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> High;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UStaticMesh*> SM_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UStaticMesh*> SM_Light;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UMaterialInterface*> MI_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UMaterialInterface*> MI_Light;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UMaterialInterface*> MI_Shadow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UNiagaraSystem*> NS_Boom;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UStaticMesh*> Gun_SM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UMaterialInterface*> Gun_MI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UNiagaraSystem*> Gun_NS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Gun_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Gun_Accurary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<float> Gun_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<TSubclassOf<AActor>> Gun_BP;


};
