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
	GamerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	GamerSpringArm->SetupAttachment(RootComponent);
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
	FVector DeltaLoc;

	//check direction and boundaries of the world on the x-axis
	if(((GetActorLocation().X <= SizeWorld) || ((StartTouchWorldLoc.X - Loc.X) < 0))
		&& ((GetActorLocation().X >= -SizeWorld) || ((StartTouchWorldLoc.X - Loc.X) > 0)))
	{
		DeltaLoc.X = (StartTouchWorldLoc.X - Loc.X) * SpeedCameraMove;
	}
	else
	{
		DeltaLoc.X = 0;
	}

	//check direction and boundaries of the world on the y-axis
	if (((GetActorLocation().Y <= SizeWorld) || ((StartTouchWorldLoc.Y - Loc.Y) < 0))
		&& ((GetActorLocation().Y >= -SizeWorld) || ((StartTouchWorldLoc.Y - Loc.Y) > 0)))
	{
		DeltaLoc.Y = (StartTouchWorldLoc.Y - Loc.Y) * SpeedCameraMove;
	}
	else
	{
		DeltaLoc.Y = 0;
	}
	
	DeltaLoc.Z = 0;

	AddActorWorldOffset(DeltaLoc);
	StartTouchWorldLoc = Loc;
}



//"BuildCreator camera move control"------------------------------------------------------------------------------>
void APlayerCamera::CameraMove(FVector2D Loc, FIntPoint ScreenSize)
{
	if ((Loc.X < (ScreenSize.X * 0.08)) && (GetActorLocation().X <= SizeWorld))
	{
		AddActorWorldOffset(GetActorRightVector() * (-1.0f) * SpeedCameraScreen);
	}
	if ((Loc.Y < (ScreenSize.Y * 0.08)) && (GetActorLocation().Y <= SizeWorld))
	{
		//AddMovementInput(GetActorForwardVector());
		AddActorWorldOffset(GetActorForwardVector() * SpeedCameraScreen);
	}
	if ((Loc.X > (ScreenSize.X * 0.92)) && (GetActorLocation().X >= -SizeWorld))
	{
		AddActorWorldOffset(GetActorRightVector() * SpeedCameraScreen);
		//AddMovementInput(GetActorRightVector());
	}
	if ((Loc.Y > (ScreenSize.Y * 0.92)) && (GetActorLocation().Y >= -SizeWorld))
	{
		AddMovementInput(GetActorForwardVector(), -1);
		AddActorWorldOffset(GetActorForwardVector() * (-1.0f) * SpeedCameraScreen);
	}
	
}



//"Touch camera zoom control"------------------------------------------------------------------------------------->
void APlayerCamera::CameraZoom(float A)
{
	if (A != 0.0f)
	{
		GamerSpringArm->TargetArmLength = GamerSpringArm->TargetArmLength - (A * SpeedCameraZoom);
		if (GamerSpringArm->TargetArmLength > LenghtMax)
		{
			GamerSpringArm->TargetArmLength = LenghtMax;
		}
		if (GamerSpringArm->TargetArmLength < LenghtMin)
		{
			GamerSpringArm->TargetArmLength = LenghtMin;
		}
		SpringArmRotate();
	}
}


//set Y-axis rotate of springarm
void APlayerCamera::SpringArmRotate()
{
	GamerSpringArm->SetRelativeRotation(FRotator(
		UKismetMathLibrary::MapRangeClamped(GamerSpringArm->TargetArmLength,
			LenghtMin, LenghtMax, RotMin, RotMax),0.0f, 0.0f));
}





//"BeginPlay. It works when game starts"------------------------------------------------------------------------->
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
}



//"Tick. It works every tick, if EverTick = true"---------------------------------------------------------------->
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



