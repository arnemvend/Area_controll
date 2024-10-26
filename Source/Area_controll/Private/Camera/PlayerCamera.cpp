// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Camera/PlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"


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
	GamerPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	GamerSpringArm->SetupAttachment(RootComponent);
	GamerCamera->SetupAttachment(GamerSpringArm);



	//"Set variables for moving"------------------------------------------------------------------------------------->
	SpeedCameraMove = 1.5;
	SpeedCameraZoom = 300.0;
	LenghtMax = 2200.0;
	LenghtMin = 1000.0;
}



//"Touch camera move control"--------------------------------------------------------------------------------------->
void APlayerCamera::StartTouchMove(FVector2D Loc)
{
	StartTouchWorldLoc = Loc;
}

void APlayerCamera::TouchMove(FVector2D Loc)
{
	TouchWorldLoc = Loc;
	FVector DeltaLoc;
	FVector ActorLocation = GetActorLocation();

	//check direction and boundaries of the world on the x-axis
	if(((ActorLocation.X <= SizeWorld) || ((StartTouchWorldLoc.X - TouchWorldLoc.X) < 0)) 
		&& ((ActorLocation.X >= -SizeWorld) || ((StartTouchWorldLoc.X - TouchWorldLoc.X) > 0)))
	{
		DeltaLoc.X = (StartTouchWorldLoc.X - TouchWorldLoc.X) * SpeedCameraMove;
	}
	else
	{
		DeltaLoc.X = 0;
	}

	//check direction and boundaries of the world on the y-axis
	if (((ActorLocation.Y <= SizeWorld) || ((StartTouchWorldLoc.Y - TouchWorldLoc.Y) < 0)) 
		&& ((ActorLocation.Y >= -SizeWorld) || ((StartTouchWorldLoc.Y - TouchWorldLoc.Y) > 0)))
	{
		DeltaLoc.Y = (StartTouchWorldLoc.Y - TouchWorldLoc.Y) * SpeedCameraMove;
	}
	else
	{
		DeltaLoc.Y = 0;
	}
	
	DeltaLoc.Z = 0;

	AddActorWorldOffset(DeltaLoc);
	StartTouchWorldLoc = TouchWorldLoc;
}



//"BuildCreator camera move control"------------------------------------------------------------------------------>
void APlayerCamera::CameraMove(FVector2D Loc, FVector2D ScreenSize)
{
	FVector ActorLocation = GetActorLocation();

	if ((Loc.X < (ScreenSize.X * 0.08)) && (ActorLocation.X <= SizeWorld))
	{
		AddMovementInput(GetActorRightVector(), -1);
	}
	if ((Loc.Y < (ScreenSize.Y * 0.08)) && (ActorLocation.Y <= SizeWorld))
	{
		AddMovementInput(GetActorForwardVector());
	}
	if ((Loc.X > (ScreenSize.X * 0.92)) && (ActorLocation.X >= -SizeWorld))
	{
		AddMovementInput(GetActorRightVector());
	}
	if ((Loc.Y > (ScreenSize.Y * 0.92)) && (ActorLocation.Y >= -SizeWorld))
	{
		AddMovementInput(GetActorForwardVector(), -1);
	}
}



//"Touch camera zoom control"------------------------------------------------------------------------------------->
void APlayerCamera::CameraZoom(float A, float B)
{
	if (A != B)
	{
		if (A < B)
		{
			GamerSpringArm->TargetArmLength =
				FMath::Clamp(GamerSpringArm->TargetArmLength + ((1 - FMath::Clamp(A / B, 0, 1)) * SpeedCameraZoom), LenghtMin, LenghtMax);
		}

		if (A > B)
		{
			GamerSpringArm->TargetArmLength =
				FMath::Clamp(GamerSpringArm->TargetArmLength + ((FMath::Clamp(B / A, 0, 1) - 1) * SpeedCameraZoom), LenghtMin, LenghtMax);
		}
	}
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

