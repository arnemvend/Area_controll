// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AreaControll_PlayerController.generated.h"

class ABuildCreator;
class AConstruction;
class AGroundActor;
class APlayerCamera;





UCLASS()
class AREA_CONTROLL_API AAreaControll_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:	
	
	AAreaControll_PlayerController();

	UPROPERTY() bool CreatorIsHere;

	UFUNCTION() void SpownCreatorFunc();
	UFUNCTION() void FindReferences();

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;
	
	
	UPROPERTY() APlayerCamera* PlayerCamera;
	UPROPERTY() AConstruction* Construction;
	UPROPERTY() ABuildCreator* BuildCreator;
	UPROPERTY() AGroundActor* GroundActor;
	UPROPERTY() TArray<AActor*> OutActors;
	

	UPROPERTY() float InitDistance;
	UPROPERTY() float CurrentDistance;
	UPROPERTY() FVector2D Position;
	UPROPERTY() FVector2D Loc1;
	UPROPERTY() FVector2D Loc2;
	

	
	UPROPERTY() bool IsPinch;
	UPROPERTY() bool CanPress;
	UPROPERTY() bool IsTouch1;
	UPROPERTY() bool IsTouch2;
	UPROPERTY() bool CanMove;


	UPROPERTY() FTimerHandle Timer0;
	UPROPERTY() FTimerHandle Timer1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<AConstruction> SpownedC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn") TSubclassOf<ABuildCreator> SpownedB;

	
	UPROPERTY() FVector MyLoc;

	UFUNCTION() void OnMouseWheelAxis(float Value); //debug function for editor

	UFUNCTION() void OnPinchAxis(float Value);
	UFUNCTION() void OnPinchPress();
	UFUNCTION() void OnPinchReleas();

	UFUNCTION() void OnQuitRelease();

	UFUNCTION() void OnTouchPress(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION() void OnTouchReleas(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION() void OnTouchMove(const ETouchIndex::Type FingerIndex, const FVector Loc);

	UFUNCTION() void BuildCreatorMove(const FVector Loc);

	UFUNCTION() void TouchToWorld(float X, float Y, TArray<AActor*> Actors);

	
	
};
