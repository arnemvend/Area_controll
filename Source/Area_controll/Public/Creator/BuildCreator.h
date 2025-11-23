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
class UAreaControll_GameInstance;
class AAreaControll_GameMode;
class AAreaRadius;



UCLASS()
class AREA_CONTROLL_API ABuildCreator : public APawn
{
	GENERATED_BODY()

public:

	ABuildCreator();

	UPROPERTY() bool IsReady;

	//"Declare function for moving this pawn"
	UFUNCTION() void MovingFunc(FVector2D Loc);


	
protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;



	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY() int8 n; // bag fix variable
	UPROPERTY() FTimerHandle Timer0; // bag fix variable
	UPROPERTY() FTimerHandle Timer1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statick Mesh") UStaticMeshComponent* StaticMesh0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") UCapsuleComponent* TriggerCapsule;
	UPROPERTY() UMaterialInterface* CreatorMaterial;
	UPROPERTY() UMaterialInterface* CreatorMaterial0;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY() UAreaControll_GameInstance* GInstance;
	UPROPERTY() AAreaControll_GameMode* GMode;
	UPROPERTY() AAreaRadius* AreaRadius;
	
	UPROPERTY() FLinearColor NewColor;
	UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial0;

	UPROPERTY() int InNumber;// Numder of inputs to Internal Thigger
	UPROPERTY() int ExNumber;// Numder of inputs to External Thigger
	UPROPERTY() bool CanBuild;

	UPROPERTY() TArray<UPrimitiveComponent*> NeutralComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> ExternalComponents;
	UPROPERTY() TArray<UPrimitiveComponent*> RadiusComponents;
	


	//"Declare event's functions and variables for their"------------------------------------------------------->
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	

	
};
