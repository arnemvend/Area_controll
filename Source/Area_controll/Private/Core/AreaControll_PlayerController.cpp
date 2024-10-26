// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Core/AreaControll_PlayerController.h"
#include "Camera/PlayerCamera.h"
#include "Creator/BuildCreator.h"
//#include "Functions/MyBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Map/GroundActor.h"
#include "Tower/Construction.h"
#include "Tower/Tower.h"



AAreaControll_PlayerController::AAreaControll_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	InitDistance = 0.00001f;

	CreatorIsHere = false;
	IsPinch = false;
	CanStart = true;
	CanPress = true;
}


void AAreaControll_PlayerController::FindReferences()
{
	GroundActor = Cast<AGroundActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGroundActor::StaticClass()));
	PlayerCamera = Cast<APlayerCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCamera::StaticClass()));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
	if (GroundActor && OutActors.Num() > 0)
	{
		OutActors.Remove(GroundActor);
		for (int i = 0; i < OutActors.Num(); i++)
		{
			if (OutActors[i]->ActorHasTag(FName(TEXT("Main"))))
			{
				MainTower = Cast<ATower>(OutActors[i]);
			}
		}
	}
	
}



void AAreaControll_PlayerController::OnMouseWheelAxis(float Value)
{
	if (PlayerCamera && Value != 0.0f)
	{
		PlayerCamera->CameraZoom(Value, 0.0f);
	}
}



void AAreaControll_PlayerController::OnPinchAxis(float Value)
{
	if (!BuildCreator && IsPinch && PlayerCamera && Value != 0.0f)
	{
		PlayerCamera->CameraZoom(Value, InitDistance);
		InitDistance = Value;
	}
}


void AAreaControll_PlayerController::OnPinchPress()
{
	if (!BuildCreator && PlayerCamera)
	{
		IsPinch = true;
		if (CanPress)
		{
			InitDistance = 1.0f;
			CanPress = false;
		}
	}
}


void AAreaControll_PlayerController::OnPinchReleas()
{
	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		IsPinch = false;
		if (CanPress)
		{
			InitDistance = 1.0f;
			CanPress = false;
		}

	}, 0.2f, false);
}


void AAreaControll_PlayerController::OnTouchPress(const ETouchIndex::Type FingerIndex, const FVector Loc)
{
	if (IsValid(BuildCreator) == false && !IsPinch)
	{
		if (CanStart)
		{
			CanStart = false;
			TouchToWorld(Loc.X, Loc.Y, OutActors);
			PlayerCamera->StartTouchMove(Position);
		}
	}
	MyLoc = Loc;
}


void AAreaControll_PlayerController::OnTouchReleas(const ETouchIndex::Type FingerIndex, const FVector Loc)
{
	CanStart = true;
	if (BuildCreator)
	{
		if (BuildCreator->IsReady)
		{
			const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		    FActorSpawnParameters SpawnParams;
		    Construction = GetWorld()->SpawnActor<AConstruction>(Spowned, BuildCreator->GetActorLocation(), SpawnRotation, SpawnParams);
			if (Construction)
			{
				Construction->Color = MainTower->YourColorGround;
			    Construction->ColorFunc();
			}

		}
		BuildCreator->DestroyerFunc();
		OutActors.Empty();
		CreatorIsHere = false;
	}
	MyLoc = Loc;
}


void AAreaControll_PlayerController::OnTouchMove(const ETouchIndex::Type FingerIndex, const FVector Loc)
{
	if (IsValid(BuildCreator) == false && !IsPinch)
	{
		TouchToWorld(Loc.X, Loc.Y, OutActors);
		PlayerCamera->TouchMove(Position);
	}
	MyLoc = Loc;
}

void AAreaControll_PlayerController::BuildCreatorMove(const FVector Loc)
{
	if (CreatorIsHere && BuildCreator)
	{
		TouchToWorld(Loc.X, Loc.Y, OutActors);
		BuildCreator->MovingFunc(Position, 1.0f);
		PlayerCamera->CameraMove(FVector2D(Loc.X, Loc.Y), GEngine->GameViewport->Viewport->GetSizeXY());
	}
}

void AAreaControll_PlayerController::TouchToWorld(float X, float Y, TArray<AActor*> Actors)
{
	FVector Start;
	FVector End;
	DeprojectScreenPositionToWorld(X, Y, Start, End);
	FHitResult HitResult;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActors(Actors);
	End = End * 100000000000000000.0f;
	if ([[maybe_unused]] bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
	{
		Position.X = HitResult.Location.X;
		Position.Y = HitResult.Location.Y;
	}
}


void AAreaControll_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BuildCreatorMove(MyLoc);
}




void AAreaControll_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FindReferences();
}




void AAreaControll_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MouseWheel", this, &AAreaControll_PlayerController::OnMouseWheelAxis);
	InputComponent->BindAxis("Pinch", this, &AAreaControll_PlayerController::OnPinchAxis);
	InputComponent->BindAction("Pinch", IE_Pressed, this, &AAreaControll_PlayerController::OnPinchPress);
	InputComponent->BindAction("Pinch", IE_Released, this, &AAreaControll_PlayerController::OnPinchReleas);
	InputComponent->BindTouch(IE_Pressed, this, &AAreaControll_PlayerController::OnTouchPress);
	InputComponent->BindTouch(IE_Released, this, &AAreaControll_PlayerController::OnTouchReleas);
	InputComponent->BindTouch(IE_Repeat, this, &AAreaControll_PlayerController::OnTouchMove);
	
}













