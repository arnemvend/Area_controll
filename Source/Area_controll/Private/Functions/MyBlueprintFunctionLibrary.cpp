// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Functions/MyBlueprintFunctionLibrary.h"

void UMyBlueprintFunctionLibrary::TouchToWorld(float X, float Y, TArray<AActor*> Actors, FVector2D& Position, APlayerController* PlayerController, UObject* WorldContextObject)
{
	FVector Start;
	FVector End;
	PlayerController->DeprojectScreenPositionToWorld(X, Y, Start, End);
	FHitResult HitResult;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActors(Actors);
	End = End * 100000000000000000.0f;
	if ([[maybe_unused]] bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, TraceParams))
	{
		Position.X = HitResult.Location.X;
		Position.Y = HitResult.Location.Y;
	}

}
