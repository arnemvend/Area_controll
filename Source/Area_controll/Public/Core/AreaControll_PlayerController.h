// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AreaControll_PlayerController.generated.h"

class ABuildCreator;
class AConstruction;
class AGroundActor;
class APlayerCamera;
class ATower;





UCLASS()
class AREA_CONTROLL_API AAreaControll_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:	
	
	AAreaControll_PlayerController();

	UPROPERTY() bool CreatorIsHere;

	UPROPERTY() ABuildCreator* BuildCreator;
	UPROPERTY() AGroundActor* GroundActor;
	UPROPERTY() TArray<AActor*> OutActors;


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;
	
	
	UPROPERTY() APlayerCamera* PlayerCamera;
	UPROPERTY() ATower* MainTower;
	UPROPERTY() AConstruction* Construction;
	

	UPROPERTY() float InitDistance;
	UPROPERTY() float CurrentDistance;
	UPROPERTY() FVector2D Position;
	UPROPERTY() FVector2D Loc1;
	UPROPERTY() FVector2D Loc2;
	

	
	UPROPERTY() bool IsPinch;
	UPROPERTY() bool CanPress;
	UPROPERTY() bool IsTouch1;
	UPROPERTY() bool IsTouch2;


	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<AConstruction> Spowned;

	
	UPROPERTY() FVector MyLoc;


	UFUNCTION() void FindReferences();


	UFUNCTION() void OnMouseWheelAxis(float Value); //debug function for editor

	UFUNCTION() void OnPinchAxis(float Value);
	UFUNCTION() void OnPinchPress();
	UFUNCTION() void OnPinchReleas();

	UFUNCTION() void OnTouchPress(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION() void OnTouchReleas(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION() void OnTouchMove(const ETouchIndex::Type FingerIndex, const FVector Loc);

	UFUNCTION() void BuildCreatorMove(const FVector Loc);

	UFUNCTION() void TouchToWorld(float X, float Y, TArray<AActor*> Actors);
	
};
