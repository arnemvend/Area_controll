// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/MainTower.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Kismet/GameplayStatics.h"


AMainTower::AMainTower()
{
	UpGun->DestroyComponent();
	MidGun->DestroyComponent();
	LowGun->DestroyComponent();

	NiagaraNet->SetEmitterEnable("Ribbon", false);
}




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
	GetWorldTimerManager().ClearTimer(Timer010);
	GetWorldTimerManager().SetTimer(Timer010, [this]()
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
					if ([[maybe_unused]] bool bHit = GetWorld()->LineTraceSingleByChannel
					    (HitResult, RunCheck, End, TraceChannel, TraceParams))
					{
						if (HitResult.Location.Z > 0 && HitResult.Component->ComponentHasTag(Name))
						{
							MassEnergy += EnergyPoint;
							//DrawDebugLine(GetWorld(), RunCheck, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
							//DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
						}
					}
					RunCheck.Y = RunCheck.Y - EnergyStep;
				}
				RunCheck.Y = UpBorder + CollisionEnergy->GetScaledCapsuleHalfHeight();
				RunCheck.X = RunCheck.X + EnergyStep;
			}
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

			if (IsValid(DTower) && DTower != this)
			{
				CheckNew(DTower);
			}
		}
	}
	DTower = nullptr;
	ActorsOfClass.Empty();
}




//"Main starts finder's operations"----------------------------------------------------------------------------->
void AMainTower::MainFinder()
{
	/*//set array for towers deactivate them
	TArray<ATower*> ActorsOfThisClass = GMode->PlayerTowers;
	ActorsOfThisClass.Append(GMode->EnemyTowers);

	if (ActorsOfThisClass.Num() > 0)
	{
		for (int i = 0; i < ActorsOfThisClass.Num(); i++)
		{
			if (IsValid(ActorsOfThisClass[i]) && IsValid(TTower) && ActorsOfThisClass[i]->ActorHasTag(Name)
				&& Main != ActorsOfThisClass[i] && MainEnemy != ActorsOfThisClass[i]
				&& ActorsOfThisClass[i]->AdressTower.Num() > TTower->AdressTower.Num())
			{
				if (ActorsOfThisClass[i]->AdressTower[TTower->Wave] == TTower->AdressTower.Last())
				{
					ActorsOfThisClass[i]->NetOff();
				}
			}
		}
	}
	ActorsOfThisClass.Empty(0);
	if (IsValid(TTower))
	{
		TTower->NetOff();
		GMode->DisabledTowers.RemoveSwap(TTower);
		TTower->Destroy();
		CheckEnergy();
	}*/
	

	GetWorldTimerManager().ClearTimer(Timer013);
	GetWorldTimerManager().SetTimer(Timer013, [this]()
	{
		NotNet = 0;
		ReFinder(Name);//search on

		GetWorldTimerManager().ClearTimer(Timer011);
		GetWorldTimerManager().SetTimer(Timer011, [this]()
		{
			CheckEnergy();

			if (GMode->DisabledTowers.Num() > 0)
			{
				if (Main == this)
				{
					MainEnemy->ReFinder(MainEnemy->Name);
				}
				if (MainEnemy == this)
				{
					Main->ReFinder(Main->Name);
				}
			}
		}, 0.1f, false);
	}, 0.1f, false);

	
}


//"Finder's operations"----------------------------------------------------------------------------->
void AMainTower::ReFinder(FName BName)
{
	//the search will stop when the number of disabled towers stops changing
	if(GMode->DisabledTowers.Num() != NotNet)
	{
		NotNet = GMode->DisabledTowers.Num();
		for( int i = 0; i < GMode->DisabledTowers.Num(); i++)
		{
			if(IsValid(GMode->DisabledTowers[i]))
			{
				GMode->DisabledTowers[i]->ReEnter(BName);
			}
		}
		ReFinder(BName);
	}
	else
	{
		Main->CheckEnergy();
	MainEnemy->CheckEnergy();
	}
	
}



//Start Net FX
void AMainTower::NetStart(FColor Color)
{
	NiagaraNet->Activate();
	NiagaraNet->SetVariableLinearColor(FName(TEXT("NetColor")), Color);
}




void AMainTower::BeginPlay()
{
	Super::BeginPlay();

	MaxEnergy = GInstance->MTr_MaxEnergy;
	MaxHealth = GInstance->MTr_MaxHealth;
	Health = MaxHealth;
	EnergyPoint = GInstance->MTr_EnergyPoint;
	EnergyStep = GInstance->MTr_EnergyStep;

	CanDamage = true;

	Influence = TriggerCapsuleInternal->GetScaledCapsuleRadius() + TriggerCapsuleExternal->GetScaledCapsuleRadius() + 10.0f;

	DTowMaterial = TowerMesh->CreateDynamicMaterialInstance(0, TowMaterial);
	DTowMaterial->GetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEmissive);
	DDiskMaterial = DiskMesh->CreateDynamicMaterialInstance(0, DiskMaterial);
	DPartShieldMaterial = PartShieldMesh->CreateDynamicMaterialInstance(0, PartShieldMaterial);
	DSphereShieldMaterial = SphereShieldMesh->CreateDynamicMaterialInstance(0, SphereShieldMaterial);

	UpBorder = GetActorLocation().Y;
	DownBorder = GetActorLocation().Y;
	RightBorder = GetActorLocation().X;
	LeftBorder = GetActorLocation().X;
	AdressTower.Add(0);

	ScaleFunc(SphereShieldMesh);
	ScaleFunc(PartShieldMesh);

	if (ActorHasTag(TEXT("Main")))
	{
		MyColor = GInstance->YourColor;
		GMode->PlayerTowers.AddUnique(this);
		GMode->PlayerMaxEnergy = MaxEnergy;
	}
	if (ActorHasTag(TEXT("MainEnemy")))
	{
		MyColor = GInstance->EnemyColor;
		GMode->EnemyTowers.AddUnique(this);
		GMode->EnemyMaxEnergy = MaxEnergy;
	}
	ColorsFunc(MyColor);
	GMode->DisabledTowers.RemoveSwap(this);

	TArray<AActor*> TowerArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainTower::StaticClass(), TowerArr);
	AMainTower* Tower;
	if (TowerArr.Num() > 0)
	{
		for (int i = 0; i < TowerArr.Num(); i++)
		{
			Tower = Cast<AMainTower>(TowerArr[i]);
			if (Tower->ActorHasTag(TEXT("Main")))
			{
				Main = Tower;
			}
			if (Tower->ActorHasTag(TEXT("MainEnemy")))
			{
				MainEnemy = Tower;
			}
		}
	}
	TowerArr.Empty(0);
	Tower = nullptr;

	CheckEnergy();

	GetWorldTimerManager().SetTimer(Timer012, [this]()
		{
			if (ActorHasTag(TEXT("Main")))
			{
				GMode->PlayerEnergy = FMath::Min(GMode->PlayerEnergy + MassEnergy, GMode->PlayerMaxEnergy);
			}
			else if (ActorHasTag(TEXT("MainEnemy")))
			{
				GMode->EnemyEnergy = FMath::Min(GMode->EnemyEnergy + MassEnergy, GMode->EnemyMaxEnergy);
			}
		}, 2.0f, true);
}
