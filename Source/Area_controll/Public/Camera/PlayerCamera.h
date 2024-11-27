// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UInputComponent;


UCLASS()
class AREA_CONTROLL_API APlayerCamera : public APawn
{
	GENERATED_BODY()

public:
	
	APlayerCamera();

	
	//"Declare function for moving this Pawn"------------------------------------------------------------------->
	UFUNCTION() void StartTouchMove(FVector2D Loc);
	UFUNCTION() void TouchMove(FVector2D Loc);
	UFUNCTION() void CameraMove(FVector2D Loc, FIntPoint ScreenSize);
	UFUNCTION() void CameraZoom(float A);


protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;



	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components") USpringArmComponent* GamerSpringArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components") UFloatingPawnMovement* GamerPawnMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components") UCameraComponent* GamerCamera;



	//"Declare variables for moving this Pawn"------------------------------------------------------------------->

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables") float SpeedCameraMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables") float SpeedCameraZoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables") float LenghtMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables") float LenghtMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float SizeWorld; //in Level BP
	UPROPERTY() FVector2D StartTouchWorldLoc;




};
