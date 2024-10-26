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

	virtual void Tick(float DeltaTime) override;

	
	//"Declare function for moving this Pawn"------------------------------------------------------------------->
	UFUNCTION(BlueprintCallable) void StartTouchMove(FVector2D Loc);
	UFUNCTION(BlueprintCallable) void TouchMove(FVector2D Loc);
	UFUNCTION(BlueprintCallable) void CameraMove(FVector2D Loc, FVector2D ScreenSize);
	UFUNCTION(BlueprintCallable) void CameraZoom(float A, float B);


protected:

	virtual void BeginPlay() override;



	//"Declare components"-------------------------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") USpringArmComponent* GamerSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UCameraComponent* GamerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components") UFloatingPawnMovement* GamerPawnMovement;



	//"Declare variables for moving this Pawn"------------------------------------------------------------------->
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float SpeedCameraMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float SpeedCameraZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float LenghtMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float LenghtMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float SizeWorld;
	FVector2D StartTouchWorldLoc;
	FVector2D TouchWorldLoc;




};
