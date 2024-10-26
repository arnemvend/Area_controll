// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuildCreator.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class UCapsuleComponent;
class UMaterialInstanceDynamic;
class UStaticMesh;

UCLASS()
class AREA_CONTROLL_API ABuildCreator : public APawn
{
	GENERATED_BODY()

public:

	ABuildCreator();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables") bool IsReady;

	UFUNCTION(BlueprintCallable) void DestroyerFunc();

	//"Declare function for moving this pawn"
	UFUNCTION(BlueprintCallable) void MovingFunc(FVector2D Loc, float Scale);


	
protected:

	virtual void BeginPlay() override;



	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* StaticMesh0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsule;
	UPROPERTY() UMaterialInterface* CreatorMaterial;
	UPROPERTY() UMaterialInterface* CreatorMaterial0;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	
	UPROPERTY() FLinearColor NewColor;
	UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial0;

	int InNumber;// Numder of inputs to Internal Thigger
	int ExNumber;// Numder of inputs to External Thigger


	//"Declare event's functions and variables for their"------------------------------------------------------->
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	

	
};
