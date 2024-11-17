// Copyright (c) Konstantin Pozdeev. All rights reserved.

#include "Core/AreaControll_PlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Camera/PlayerCamera.h"
#include "Creator/BuildCreator.h"
#include "Kismet/GameplayStatics.h"
#include "Map/GroundActor.h"
#include "Tower/Construction.h"
#include "Tower/Tower.h"







AAreaControll_PlayerController::AAreaControll_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	InitDistance = 0.0f;
	CreatorIsHere = false;
	IsPinch = false;
	CanPress = true;
	IsTouch1 = false;
	IsTouch2 = false;
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
	if (PlayerCamera && Value != 0.0f && (IsValid(BuildCreator) == false || CreatorIsHere == false))
	{
		PlayerCamera->CameraZoom(Value);
	}
}



void AAreaControll_PlayerController::OnPinchAxis(float Value)
{
	GetInputTouchState(ETouchIndex::Touch1, Loc1.X, Loc1.Y, IsTouch1);
	GetInputTouchState(ETouchIndex::Touch2, Loc2.X, Loc2.Y, IsTouch2);

	if (PlayerCamera && Value != 0.0f && (IsValid(BuildCreator) == false || CreatorIsHere == false))
	{
		if (IsTouch1 && IsTouch2)
		{
			if (InitDistance == 0.0f)
			{
				InitDistance = (Loc2 - Loc1).Length();
			}
			CurrentDistance = (Loc2 - Loc1).Length();
			PlayerCamera->CameraZoom(FMath::Clamp(((CurrentDistance - InitDistance) / InitDistance) * 2.5f, -1.0f, 1.0f));
			InitDistance = CurrentDistance;
		}
	}
}



void AAreaControll_PlayerController::OnPinchPress()
{
	GetInputTouchState(ETouchIndex::Touch1, Loc1.X, Loc1.Y, IsTouch1);
	GetInputTouchState(ETouchIndex::Touch2, Loc2.X, Loc2.Y, IsTouch2);

	if (IsValid(BuildCreator) == false && PlayerCamera && CreatorIsHere == false)
	{
		IsPinch = true;
		if (CanPress)
		{
			InitDistance = 0.0f;
			CurrentDistance = 0.0f;
			CanPress = false;
		}
	}
}


void AAreaControll_PlayerController::OnPinchReleas()
{
	InitDistance = 0.0f;
	CurrentDistance = 0.0f;
	GetWorldTimerManager().SetTimer(Timer0, [this]()
	{
		CanPress = true;
		IsPinch = false;
	}, 0.1f, false);
}


void AAreaControll_PlayerController::OnTouchPress(const ETouchIndex::Type FingerIndex, const FVector Loc)
{
	GetInputTouchState(ETouchIndex::Touch1, Loc1.X, Loc1.Y, IsTouch1);
	GetInputTouchState(ETouchIndex::Touch2, Loc2.X, Loc2.Y, IsTouch2);

	if (IsValid(BuildCreator) == false  && IsPinch == false  && IsTouch1  && IsTouch2 == false && CreatorIsHere == false)
	{
		TouchToWorld(Loc.X, Loc.Y, OutActors);
		PlayerCamera->StartTouchMove(Position);
		MyLoc = Loc;
	}
}


void AAreaControll_PlayerController::OnTouchReleas(const ETouchIndex::Type FingerIndex, const FVector Loc)
{
	if (BuildCreator && CreatorIsHere)
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
	GetInputTouchState(ETouchIndex::Touch1, Loc1.X, Loc1.Y, IsTouch1);
	GetInputTouchState(ETouchIndex::Touch2, Loc2.X, Loc2.Y, IsTouch2);

	if (IsValid(BuildCreator) == false && IsPinch == false && IsTouch1 && IsTouch2 == false && CreatorIsHere == false)
	{
		TouchToWorld(Loc.X, Loc.Y, OutActors);
		PlayerCamera->TouchMove(Position);
	}
	if (IsPinch == false && IsTouch1 && IsTouch2 == false && CreatorIsHere && BuildCreator)
	{
		BuildCreatorMove(Loc);
	}
	MyLoc = Loc;
}

void AAreaControll_PlayerController::BuildCreatorMove(const FVector Loc)
{
	TouchToWorld(Loc.X, Loc.Y, OutActors);
	BuildCreator->MovingFunc(Position);
	PlayerCamera->CameraMove(FVector2D(Loc.X, Loc.Y), GEngine->GameViewport->Viewport->GetSizeXY());
}

void AAreaControll_PlayerController::TouchToWorld(float X, float Y, TArray<AActor*> Actors)
{
	FVector Start;
	FVector End;
	DeprojectScreenPositionToWorld(X, Y, Start, End);
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActors(Actors);
	End = End * 1000000000000000000000000000000000000.0f;
	if ([[maybe_unused]] bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		Position.X = HitResult.Location.X;
		Position.Y = HitResult.Location.Y;
		//DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
		//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
		//UE_LOG(LogTemp, Warning, TEXT("EndLoc: X = %f, Y = %f"), Position.X, Position.Y);
	}
}




void AAreaControll_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	if (BuildCreator && FVector2D(MyLoc.X, MyLoc.Y) == Loc1 && BuildCreator->GetActorLocation().Z > 0.0f)
	{
		BuildCreatorMove(FVector(MyLoc));
	}
}




void AAreaControll_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FindReferences();
}

void AAreaControll_PlayerController::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
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













