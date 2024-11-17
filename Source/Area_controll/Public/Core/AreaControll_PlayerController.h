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


	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;





protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") ABuildCreator* BuildCreator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") AGroundActor* GroundActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") APlayerCamera* PlayerCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") ATower* MainTower;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") AConstruction* Construction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References") TArray<AActor*> OutActors;

	UPROPERTY() float InitDistance;
	UPROPERTY() float CurrentDistance;
	UPROPERTY() FVector2D Position;
	UPROPERTY() FVector2D Loc1;
	UPROPERTY() FVector2D Loc2;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables") bool CreatorIsHere;
	UPROPERTY() bool IsPinch;
	UPROPERTY() bool CanPress;
	UPROPERTY() bool IsTouch1;
	UPROPERTY() bool IsTouch2;


	UPROPERTY() FTimerHandle Timer0;

	UPROPERTY() TSubclassOf<AConstruction> Spowned;

	
	UPROPERTY() FVector MyLoc;


	UFUNCTION() void FindReferences();


	UFUNCTION() void OnMouseWheelAxis(float Value);

	UFUNCTION() void OnPinchAxis(float Value);
	UFUNCTION() void OnPinchPress();
	UFUNCTION() void OnPinchReleas();

	UFUNCTION() void OnTouchPress(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION(BlueprintCallable) void OnTouchReleas(const ETouchIndex::Type FingerIndex, const FVector Loc);
	UFUNCTION() void OnTouchMove(const ETouchIndex::Type FingerIndex, const FVector Loc);

	UFUNCTION() void BuildCreatorMove(const FVector Loc);

	UFUNCTION() void TouchToWorld(float X, float Y, TArray<AActor*> Actors);
	
};
