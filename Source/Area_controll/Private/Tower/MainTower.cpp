// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/MainTower.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"







//-----------------------THERE ARE FUNCTIONS FOR CHECK ENERGY-------------------------------------------------//
//"Set border of area controll"-------------------------------------------------------------------------------->
void AMainTower::CheckNew(ATower* Tower)
{
	//defining the boundaries of the rectangle to check
	if (Tower->GetActorLocation().Y > UpBorder)
	{
		UpBorder = Tower->GetActorLocation().Y;
	}
	if (Tower->GetActorLocation().Y < DownBorder)
	{
		DownBorder = Tower->GetActorLocation().Y;
	}
	if (Tower->GetActorLocation().X > RightBorder)
	{
		RightBorder = Tower->GetActorLocation().X;
	}
	if (Tower->GetActorLocation().X < LeftBorder)
	{
		LeftBorder = Tower->GetActorLocation().X;
	}
}




//"Calculation energy pool"------------------------------------------------------------------------------------->
void AMainTower::CheckEnergy()
{
	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			MassEnergy = 0;
			FVector RunCheck = FVector(LeftBorder - CollisionEnergy->GetScaledCapsuleRadius(),
				UpBorder + CollisionEnergy->GetScaledCapsuleHalfHeight(), 140.0f);
			//checking for hits to the energy trigger in the rectangle area
			while (RunCheck.X <= RightBorder + CollisionEnergy->GetScaledCapsuleRadius())
			{
				while (RunCheck.Y >= DownBorder - CollisionEnergy->GetScaledCapsuleRadius())
				{
					FVector End;
					End.X = RunCheck.X;
					End.Y = RunCheck.Y;
					End.Z = -200.0f;
					FHitResult HitResult;
					ECollisionChannel TraceChannel = ECC_Camera;
					FCollisionQueryParams TraceParams;
					if ([[maybe_unused]] bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RunCheck, End, TraceChannel, TraceParams))
					{
						if (HitResult.Location.Z > 0 && HitResult.Component->ComponentHasTag(Name))
						{
							MassEnergy = MassEnergy + EnergyPoint;
							//DrawDebugLine(GetWorld(), RunCheck, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
							//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
						}
					}
					RunCheck.Y = RunCheck.Y - EnergyStep;
				}
				RunCheck.Y = UpBorder + CollisionEnergy->GetScaledCapsuleHalfHeight();
				RunCheck.X = RunCheck.X + EnergyStep;
			}
			//UE_LOG(LogTemp, Warning, TEXT("%d"), MassEnergy);
		}, 0.5f, false);
}




// "Reload area for energy"------------------------------------------------------------------------------------->
void AMainTower::ReloadEnergy(FName DName)
{
	//updating the dimensions of the energy calculation rectangle
	UpBorder = GetActorLocation().Y;
	DownBorder = GetActorLocation().Y;
	RightBorder = GetActorLocation().X;
	LeftBorder = GetActorLocation().X;
	TArray<AActor*> ActorsOfClass;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), DName, ActorsOfClass);
	//overwriting borders by tower coordinates
	ATower* DTower{};
	if (ActorsOfClass.Num() > 0)
	{
		for (int i = 0; i < ActorsOfClass.Num(); i++)
		{
			if (ActorsOfClass[i])
			{
				DTower = Cast<ATower>(ActorsOfClass[i]);
			}

			if (DTower && DTower != this)
			{
				CheckNew(DTower);
			}
		}
	}
	DTower = nullptr;
	ActorsOfClass.Empty();
}




//"Main starts finder's operations"----------------------------------------------------------------------------->
void AMainTower::MainFinder(ATower* TTower)
{

	//GetWorldTimerManager().SetTimer(Timer1, [this, TTower]()
	//{
			TArray<AActor*> ActorsOfThisClass;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), ActorsOfThisClass);
			//set array for towers deactivate them
			ATower* PoofTower{};
			if (ActorsOfThisClass.Num() > 0)
			{
				for (int i = 0; i < ActorsOfThisClass.Num(); i++)
				{
					if (ActorsOfThisClass[i])
					{
						PoofTower = Cast<ATower>(ActorsOfThisClass[i]);
					}

					if (PoofTower && TTower && PoofTower->ActorHasTag(Name) && Main != PoofTower && MainEnemy != PoofTower
						&& PoofTower->AdressTower.Num() > TTower->AdressTower.Num())
					{
						if (PoofTower->AdressTower[TTower->Wave] == TTower->AdressTower.Last())
						{
							PoofTower->NetOff();
						}
					}
				}
			}
			PoofTower = nullptr;
			if (TTower)
			{
				TTower->NetOff();
				TTower->Destroy();
				CheckEnergy();
			}

			NotNet = 0;
			ReFinder(Name);//search on


	//}, 0.01f, false);

	GetWorldTimerManager().SetTimer(Timer2, [this]()
	{
			if (NextTowers.Num() > 0)
			{
				for (int i = 0; i < NextTowers.Num(); i++)
				{
					if (NextTowers[i])
					{
						if (Main == this)
						{
							ATower* Tow = NextTowers[i];
							Tow->ReEnter(MainEnemy->Name);
						}
						if (MainEnemy == this)
						{
							ATower* Tow = NextTowers[i];
							Tow->ReEnter(Main->Name);
						}
					}
				}
			}
	}, 0.1f, false);
}


//"Finder's operations"----------------------------------------------------------------------------->
void AMainTower::ReFinder(FName BName)
{
	NextTowers.Empty();
	TArray<AActor*> OfThisClass;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), OfThisClass);
	ATower* RSTower{};
	if (OfThisClass.Num() > 0)
	{
		for (int i = 0; i < OfThisClass.Num(); i++)
		{
			if (OfThisClass[i])
			{
				RSTower = Cast<ATower>(OfThisClass[i]);
			}
			if (RSTower && RSTower->Tags.Num() == 0 && Main != RSTower && MainEnemy != RSTower)
			{
				NextTowers.AddUnique(RSTower);
			}
		}
	}
	RSTower = nullptr;
	//the search will stop when the number of disabled towers stops changing
	if(NextTowers.Num() != NotNet)
	{
		NotNet = NextTowers.Num();
		for( int i = 0; i < NextTowers.Num(); i++)
		{
			if(NextTowers[i])
			{
				ATower* Tow = NextTowers[i];
				Tow->ReEnter(BName);
			}
		}
		NextTowers.Empty();
		ReFinder(BName);
	}
	Main->CheckEnergy();
	MainEnemy->CheckEnergy();
}