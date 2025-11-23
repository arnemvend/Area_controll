// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Camera/PlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


APlayerCamera::APlayerCamera()
{
 	
	PrimaryActorTick.bCanEverTick = false;



	//"Set default variables of this Pawn"--------------------------------------------------------------------------->
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;



	//"Set default components"--------------------------------------------------------------------------------------->
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GamerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	GamerSpringArm->SetupAttachment(RootComponent);

	GamerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	GamerCamera->SetupAttachment(GamerSpringArm);



	//"Set variables for moving"------------------------------------------------------------------------------------->
	SpeedCameraMove = 1.5f;
	SpeedCameraScreen = 1.5f;
	SpeedCameraZoom = 300.0f;
	LenghtMax = 2200.0f;
	LenghtMin = 1000.0f;
}



//"Touch camera move control"--------------------------------------------------------------------------------------->
//Set start value
void APlayerCamera::StartTouchMove(FVector2D Loc)
{
	StartTouchWorldLoc = Loc;
}


void APlayerCamera::TouchMove(FVector2D Loc)
{
	if (StartTouchWorldLoc == Loc)
	{
		return;
	}

	FVector DeltaLoc = FVector::ZeroVector;

	DeltaLoc.X = (StartTouchWorldLoc.X - Loc.X) * SpeedCameraMove;
	DeltaLoc.Y = (StartTouchWorldLoc.Y - Loc.Y) * SpeedCameraMove;

	float CurrentSize = SizeWorld - Offset;
	
	FVector NewLoc = GetActorLocation() + DeltaLoc;
	NewLoc.X = FMath::Clamp(NewLoc.X, -CurrentSize, CurrentSize);
	NewLoc.Y = FMath::Clamp(NewLoc.Y, -CurrentSize, CurrentSize);

	SetActorLocation(NewLoc, false, nullptr, ETeleportType::ResetPhysics);
	
	StartTouchWorldLoc = Loc;
}



//"BuildCreator camera move control"------------------------------------------------------------------------------>
void APlayerCamera::CameraMove(FVector2D Loc, FIntPoint ScreenSize)
{
	float CurrentSize = SizeWorld - Offset;
	if ((Loc.X < (ScreenSize.X * 0.08)) && (GetActorLocation().X <= CurrentSize))
	{
		AddActorWorldOffset(GetActorRightVector() * (-1.0f) * SpeedCameraScreen, 
			false, nullptr, ETeleportType::ResetPhysics);
	}
	else if ((Loc.X > (ScreenSize.X * 0.92)) && (GetActorLocation().X >= -CurrentSize))
	{
		AddActorWorldOffset(GetActorRightVector() * SpeedCameraScreen,
			false, nullptr, ETeleportType::ResetPhysics);
	}
	if ((Loc.Y < (ScreenSize.Y * 0.08)) && (GetActorLocation().Y <= CurrentSize))
	{
		AddActorWorldOffset(GetActorForwardVector() * SpeedCameraScreen, 
			false, nullptr, ETeleportType::ResetPhysics);
	}
	else if ((Loc.Y > (ScreenSize.Y * 0.92)) && (GetActorLocation().Y >= -CurrentSize))
	{
		AddActorWorldOffset(GetActorForwardVector() * (-1.0f) * SpeedCameraScreen, 
			false, nullptr, ETeleportType::ResetPhysics);
	}
}



//"Touch camera zoom control"------------------------------------------------------------------------------------->
void APlayerCamera::CameraZoom(float A)
{
	if (FMath::Abs(A) >= 0.1f)
	{
		GamerSpringArm->TargetArmLength = FMath::Clamp(GamerSpringArm->TargetArmLength - (A * SpeedCameraZoom), LenghtMin, LenghtMax);
		SetWorldSizeOffset();
		SpringArmRotate();
	}
}





void APlayerCamera::SetWorldSizeOffset()
{
	float Alpha = FMath::Clamp((GamerSpringArm->TargetArmLength - LenghtMin) / (LenghtMax - LenghtMin), 0.0f, 1.0f);
	Offset = FMath::Lerp(MinOffset, MaxOffset, Alpha);
}





//set Y-axis rotate of springarm
void APlayerCamera::SpringArmRotate()
{
	GamerSpringArm->SetRelativeRotation(FRotator(
		UKismetMathLibrary::MapRangeClamped(GamerSpringArm->TargetArmLength,
			LenghtMin, LenghtMax, RotMin, RotMax),0.0f, 0.0f), 
		false, nullptr, ETeleportType::ResetPhysics);
}





//"BeginPlay. It works when game starts"------------------------------------------------------------------------->
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	MaxOffset = LenghtMax * FMath::Tan(FMath::DegreesToRadians(GamerCamera->FieldOfView / 2.0f));
	MinOffset = LenghtMin * FMath::Tan(FMath::DegreesToRadians(GamerCamera->FieldOfView / 2.0f));

	if (FMath::IsNearlyEqual(LenghtMax, LenghtMin))
	{
		LenghtMax = LenghtMin + 1000.0f;
	}

	GamerSpringArm->TargetArmLength = FMath::Clamp(GamerSpringArm->TargetArmLength, LenghtMin, LenghtMax);

	SpringArmRotate();
}



//"Tick. It works every tick, if EverTick = true"---------------------------------------------------------------->
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



