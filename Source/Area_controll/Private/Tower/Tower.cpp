// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/Tower.h"
#include "Boom.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/PreloadActor.h"
#include "HUD/TowerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tower/MainTower.h"


ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = false;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//"Set parameters of Static Meshes"--------------------------------------------------------------------------->
	//Mesh of the main tower
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/EnergyTower.EnergyTower'"));
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	TowerMesh->SetupAttachment(RootComponent);
	TowerMesh->SetStaticMesh(MyStaticMesh);
	TowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Tower.MI_Tower'"));
	TowerMesh->SetMaterial(0, TowMaterial);

	//The disk of the earth under the tower
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Disk.Disk'"));
	DiskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiskMesh"));
	DiskMesh->SetupAttachment(RootComponent);
	DiskMesh->SetStaticMesh(MyStaticMesh);
	DiskMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Disk.MI_Disk'"));
	DiskMesh->SetMaterial(0, DiskMaterial);

	//Part of the shield visual for the neighboring tower
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/InPartShield.InPartShield'"));
	PartShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartShieldMesh"));
	PartShieldMesh->SetupAttachment(RootComponent);
	PartShieldMesh->SetStaticMesh(MyStaticMesh);
	PartShieldMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Shield.MI_Shield'"));
	PartShieldMesh->SetMaterial(0, PartShieldMaterial);

	//Mesh of the energy shield visual
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Inshield.Inshield'"));
	SphereShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereShieldMesh"));
	SphereShieldMesh->SetupAttachment(RootComponent);
	SphereShieldMesh->SetStaticMesh(MyStaticMesh);
	SphereShieldMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_Shield.MI_Shield'"));
	SphereShieldMesh->SetMaterial(0, SphereShieldMaterial);





	//"Set parameters of triggers"------------------------------------------------------------------------------->
	//Internal trigger
	TriggerCapsuleInternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InternalCollisionComponent"));
	TriggerCapsuleInternal->SetupAttachment(RootComponent);
	TriggerCapsuleInternal->SetCapsuleRadius(28.0f);
	TriggerCapsuleInternal->SetCapsuleHalfHeight(70.0f);

	//External trigger
	TriggerCapsuleExternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ExternalCollisionComponent"));
	TriggerCapsuleExternal->SetupAttachment(RootComponent);
	TriggerCapsuleExternal->SetCapsuleRadius(200.0f);
	TriggerCapsuleExternal->SetCapsuleHalfHeight(300.0f);

	//Trigger for energy area
	CollisionEnergy = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionEnergyComponent"));
	CollisionEnergy->SetupAttachment(RootComponent);
	CollisionEnergy->SetCapsuleRadius(100.0f);
	CollisionEnergy->SetCapsuleHalfHeight(100.0f);





	//"Set parameters of Niagara"------------------------------------------------------------------------------->
	NiagaraNet = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraNet->SetupAttachment(RootComponent);
	NiagaraNet->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	NiagaraNetSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Net.NI_Net'"));
	NiagaraNet->SetAsset(NiagaraNetSystem);




	//"Set parameters of Timeline"------------------------------------------------------------------------------>
	//timeline for the main shield
	ShieldTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("ShieldTimeLine"));
	ShieldTimeLine->SetLooping(false);
	ShieldTimeLine->SetComponentTickInterval(0.04f);

	//timeline for the secondary shield
	PShieldTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("PShieldTimeLine"));
	PShieldTimeLine->SetLooping(false);
	PShieldTimeLine->SetComponentTickInterval(0.04f);
	
	




	//"Set other variables"------------------------------------------------------------------------------------->
	ChildTowers.Empty(); //The towers are further down the network
	TemporaryTower = nullptr; //The tower is higher on the network
	TW = nullptr;
	Wave = 0;
	MaxWave = 0;
	NotNet = 0;
	NameMain = FName(TEXT("Main"));
	NameMainEnemy = FName(TEXT("MainEnemy"));
	IsProcessRun = false;
	IsClicked = false;
	IsRepeater = false;
	CanDie = true;
	


	OnTakeAnyDamage.AddDynamic(this, &ATower::OnTakeDamage);
	OnInputTouchBegin.AddDynamic(this, &ATower::Touch);
}



void ATower::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (SphereShieldMesh->IsVisible() == false && PartShieldMesh->IsVisible() == false && Health > 0)
	{
		Health = Health - Damage;
		SetDamageFunc();
	}
}



//"Change colors"------------------------------------------------------------------------------------------------>
void ATower::ColorsFunc(FColor Color)
{
	DDiskMaterial->SetVectorParameterValue(TEXT("Color_Param"), Color);
	if (IsRepeater == false)
	{
		DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 1.0f);
	}

	DTowMaterial->SetVectorParameterValue(TEXT("LightParam"), Color);
	DTowMaterial->SetVectorParameterValue(TEXT("ClickColor"), Color);
	DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
	
	DTowMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEmissive);

	DPartShieldMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
	
	DSphereShieldMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
}



void ATower::SetDamageFunc()
{
	DTowMaterial->SetScalarParameterValue(TEXT("Damage"), Health / MaxHealth);
	if (Health <= 0)
	{
		TowerDestroy();
	}
}




//"Functions fot touch actor"------------------------------------------------------------------------------------>
//the function of removing the selection
void ATower::DeTouch()
{
	if (IsClicked)
	{
		DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
		float Op; 
		DDiskMaterial->GetScalarParameterValue(TEXT("EmissiveMultiply"), Op);
		DDiskMaterial->SetScalarParameterValue(TEXT("EmissiveMultiply"), Op / 2);
		if (TW && TW->IsInViewport())
		{
			TW->RemoveFromParent();
			TW->Reset();
		}
		IsClicked = false;
	}
}

//The function of marking the selection
void ATower::Touch(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	if (ActorHasTag(TEXT("Your")))
	{
		if (IsClicked)
		{
			ATower::DeTouch();
		}
		else
		{
			//search for a highlighted tower and deselect it
			TArray<AActor*> UntouchActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), UntouchActors);
			ATower* UntouchTower{};
			if (UntouchActors.Num() > 0)
			{
				for (int i = 0; i < UntouchActors.Num(); i++)
				{
					if (UntouchActors[i])
					{
						UntouchTower = Cast<ATower>(UntouchActors[i]);
					}
					if (UntouchTower && UntouchTower != this && UntouchTower->IsClicked == true)
					{
						UntouchTower->DeTouch();
					}
				}
			}
			UntouchTower = nullptr;
			DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 0.0f);
			float Op;
			DDiskMaterial->GetScalarParameterValue(TEXT("EmissiveMultiply"), Op);
			DDiskMaterial->SetScalarParameterValue(TEXT("EmissiveMultiply"), Op * 2);
			IsClicked = true;
			//if the tower has its own, the menu is turned on
			if (ActorHasTag(FName(TEXT("Your"))))
			{
				if (TW && TW->IsInViewport() == false)
				{
					TW->MyTower = this;
					TW->StartEvent();
					TW->AddToViewport();
				}
			}
		}
	}
}


//"On repeater function"------------------------------------------------------------------------------------->
void ATower::Repeater()
{
	if(IsProcessRun == false)
	{
		IsRepeater = true;
		if (SphereShieldMesh->IsVisible() == true)
		{
			ShieldCreate();
		}
		TriggerCapsuleExternal->SetCapsuleRadius(TriggerCapsuleExternal->GetScaledCapsuleRadius() * 2.0f);
		TriggerCapsuleExternal->SetCapsuleHalfHeight(TriggerCapsuleExternal->GetScaledCapsuleHalfHeight() * 2.0f);
		CollisionEnergy->SetCapsuleRadius(1.0f);
		CollisionEnergy->SetCapsuleHalfHeight(1.0f);
		DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 0.0f);
		TowerMesh->SetStaticMesh(PActor->RMesh);
		Influence = TriggerCapsuleInternal->GetScaledCapsuleRadius() + TriggerCapsuleExternal->GetScaledCapsuleRadius();
		//NiagaraNet->SetVariableInt(FName(TEXT("SpawnUp")), 2);
		//restarting the search for disabled towers
		if (Name == FName(TEXT("Your")))
		{
			Main->NotNet = 0;
			Main->ReFinder(Main->Name);
			Main->CheckEnergy();
		}
		if (Name == FName(TEXT("YourEnemy")))
		{
			MainEnemy->NotNet = 0;
			MainEnemy->ReFinder(MainEnemy->Name);
			MainEnemy->CheckEnergy();
		}

		//NiagaraRepeater->SetAsset(NiagaraSystem);
		NiagaraRepeater = UNiagaraFunctionLibrary::SpawnSystemAttached(PActor->RNSystem, this->RootComponent, NAME_None, FVector::ZeroVector,
			FRotator(0.0f, -22.5f, 0.0f),
			EAttachLocation::Type::KeepRelativeOffset, true);
		if (ActorHasTag(TEXT("Your")))
		{
			NiagaraRepeater->SetVariableLinearColor(TEXT("Color"), PActor->YourColor);
		}
		else
		{
			NiagaraRepeater->SetVariableLinearColor(TEXT("Color"), PActor->EnemyColor);
		}
		GetWorldTimerManager().SetTimer(Timer3, [this]()
		{
				NiagaraRepeater->DeactivateImmediate();
				NiagaraRepeater->DestroyComponent();
		}, 1.5f, false);
	}
}









//-----------------------THERE ARE FUNCTIONS FOR CHECK NET OF TOWERS--------------------------------------------//
void ATower::Start(bool IsYour)
{
	//max distance between Towers in net
	Influence = TriggerCapsuleInternal->GetScaledCapsuleRadius() + TriggerCapsuleExternal->GetScaledCapsuleRadius() + 10.0f;

	DTowMaterial = TowerMesh->CreateDynamicMaterialInstance(0, TowMaterial);
	DTowMaterial->GetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEmissive);
	DDiskMaterial = DiskMesh->CreateDynamicMaterialInstance(0, DiskMaterial);
	DPartShieldMaterial = PartShieldMesh->CreateDynamicMaterialInstance(0, PartShieldMaterial);
	DSphereShieldMaterial = SphereShieldMesh->CreateDynamicMaterialInstance(0, SphereShieldMaterial);

	IsMainFunc(IsYour);

	ScaleFunc(SphereShieldMesh);
	ScaleFunc(PartShieldMesh);
}


//"Function is setter Main and MainEnemy Actors"----------------------------------------------------------------->
void ATower::IsMainFunc(bool IsYour)
{
	//the search takes place by tag, depending on the creator
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), NameMain, ActorsWithTag);
	if (ActorsWithTag.Num() > 0 && ActorsWithTag[0])
	{
		Main = Cast<AMainTower>(ActorsWithTag[0]);//set Main reference
		if (Main == this)
		{
			ColorsFunc(PActor->YourColor);
			Main->UpBorder = GetActorLocation().Y;
			Main->DownBorder = GetActorLocation().Y;
			Main->RightBorder = GetActorLocation().X;
			Main->LeftBorder = GetActorLocation().X;
			AdressTower.Add(0);
			Main->CheckEnergy();
		}
	}
	ActorsWithTag.Empty();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), NameMainEnemy, ActorsWithTag);
	if (ActorsWithTag.Num() > 0 && ActorsWithTag[0])
	{
		MainEnemy = Cast<AMainTower>(ActorsWithTag[0]);//set MainEnemy reference
		if (MainEnemy == this)
		{
			ColorsFunc(PActor->EnemyColor);
			MainEnemy->UpBorder = GetActorLocation().Y;
			MainEnemy->DownBorder = GetActorLocation().Y;
			MainEnemy->RightBorder = GetActorLocation().X;
			MainEnemy->LeftBorder = GetActorLocation().X;
			AdressTower.Add(0);
			MainEnemy->CheckEnergy();
		}
	}
	//network search
	if (Main != this && MainEnemy != this)
	{
		if (IsYour)
		{
			ReEnter(Main->Name); 
			if (Tags.Num() == 0)
			{
				ReEnter(MainEnemy->Name);
			}
		}
		else
		{
			ReEnter(MainEnemy->Name);
			if (Tags.Num() == 0)
			{
				ReEnter(Main->Name);
			}
		}
		
	}
	ActorsWithTag.Empty();
}


//"This function finds Towers in Network"----------------------------------------------------------------------->
void ATower::ReEnter(FName AName)
{
	if (Main != this && MainEnemy != this)
	{
		StepTowers.Empty();
		ChildTowers.Empty();
		TArray<AActor*> ActorsOfClass;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), AName, ActorsOfClass);
		ATower* TempTower{};
		//set arrow of towers with tag "Name" in the area of influence
		if (ActorsOfClass.Num() > 0)
		{
			for (int i = 0; i < ActorsOfClass.Num(); i++)
			{
				if (ActorsOfClass[i])
				{
					TempTower = Cast<ATower>(ActorsOfClass[i]);
				}
				if (TempTower && TempTower != this
					&& (GetHorizontalDistanceTo(TempTower) <= TempTower->Influence || GetHorizontalDistanceTo(TempTower) <= Influence))
				{
					StepTowers.AddUnique(TempTower);
				}
			}
		}
		//check distance to other tower
		CheckStep(MaxWave);
		TempTower = nullptr;
	}
}


//"this function finds the nearest tower in a generation"-------------------------------------------------------->
void ATower::CheckStep(int MWave)
{
	int Distance = 5000;
	int a = 0;
	if (StepTowers.Num() > 0)
	{
		for (int i = 0; i < StepTowers.Num(); i++)
		{
			//the "StepTowers" array is defined in the "ReEnter" function
			if (StepTowers[i] && GetHorizontalDistanceTo(StepTowers[i]) < Distance)
			{
				if (StepTowers[i]->Wave == MaxWave)
				{
					TemporaryTower = StepTowers[i];
					Distance = GetHorizontalDistanceTo(TemporaryTower);
					a++;
				}
				if (StepTowers[i]->Wave > MaxWave)
				{
					a++;
				}
			}
		}
	}
	if (TemporaryTower)
	{
		SetParam();
		StepTowers.Empty();
	}
	else
	{
		if (a > 0)
		{
			//if there are towers in a generation, but there are no suitable ones, we repeat the search in the next generation
			MaxWave++;
			CheckStep(MaxWave);
		}
		else
		{
			StepTowers.Empty();
			NetOff();
		}
	}
}


//"this function passes parameters based on the parameters of the parent tower"---------------------------------->
void ATower::SetParam()
{
	if (TemporaryTower)
	{
		int MaxIndex = 0;
		Name = TemporaryTower->Name;
		if (TemporaryTower->TW)
		{
			TW = TemporaryTower->TW;
		}
		Tags.Empty();
		Tags.Add(Name);
		Wave = TemporaryTower->Wave + 1;
		AdressTower += TemporaryTower->AdressTower;
		if (TemporaryTower->ChildTowers.Num() > 0)
		{
			for (int i = 0; i < TemporaryTower->ChildTowers.Num(); i++)
			{
				if (TemporaryTower->ChildTowers[i] && MaxIndex != TemporaryTower->ChildTowers[i]->AdressTower.Last())
				{
					break;
				}
				else
				{
					MaxIndex++;
				}
			}
		}
		TemporaryTower->ChildTowers.AddUnique(this);
		AdressTower.Add(MaxIndex);
		FVector Nearest = TemporaryTower->GetActorLocation();
		NiagaraNet->Activate();
		if (TemporaryTower->ChildTowers.Num() == 1)
		{
			if (TemporaryTower == Main)
			{
				Main->NetStart(PActor->YourColor);
			}
			if (TemporaryTower == MainEnemy)
			{
				MainEnemy->NetStart(PActor->EnemyColor);
			}
		}
		NiagaraNet->SetVariablePosition(FName(TEXT("EndVector")), 
			FVector(Nearest.X, Nearest.Y, TemporaryTower->NiagaraNet->GetRelativeLocation().Z));
		InCollName = TemporaryTower->InCollName;
		TriggerCapsuleInternal->ComponentTags.Empty();
		TriggerCapsuleInternal->ComponentTags.AddUnique(InCollName);
		ExCollName = TemporaryTower->ExCollName;
		TriggerCapsuleExternal->ComponentTags.Empty();
		TriggerCapsuleExternal->ComponentTags.AddUnique(ExCollName);
	}
	
	if (IsRepeater == false)
	{
		CollisionEnergy->ComponentTags.AddUnique(Name);
		CollisionEnergy->SetCapsuleRadius(100.0f);
		CollisionEnergy->SetCapsuleHalfHeight(100.0f);
	}
	if (Name == FName(TEXT("Your")))
	{
		NiagaraNet->SetVariableLinearColor(FName(TEXT("NetColor")), PActor->YourColor);
		ColorsFunc(PActor->YourColor);
		Main->NotNet = 0;
		Main->ReFinder(Main->Name);
		Main->CheckNew(this);
	}
	if (Name == FName(TEXT("YourEnemy")))
	{
		NiagaraNet->SetVariableLinearColor(FName(TEXT("NetColor")), PActor->EnemyColor);
		ColorsFunc(PActor->EnemyColor);
		MainEnemy->NotNet = 0;
		MainEnemy->ReFinder(MainEnemy->Name);
		MainEnemy->CheckNew(this);
	}
	
}









//-----------------------THERE ARE FUNCTIONS FOR ENERGY SHIELD--------------------------------------------------//
//"It is main function for create shield"------------------------------------------------------------------------>
void ATower::ShieldCreate()
{
	if (IsProcessRun == false && IsRepeater == false)
	{
		TlCallbackCreateShield.BindUFunction(this, FName("AmountFunc"));
		if (CreateShield)
		{
			ShieldTimeLine->AddInterpFloat(CreateShield, TlCallbackCreateShield);
		}
		if (SphereShieldMesh->IsVisible() == false)
		{
			if (PartShieldMesh->GetVisibleFlag() == true)
			{
				PartShieldMesh->SetVisibility(false);
				ScaleFunc(PartShieldMesh);
			}
			IsProcessRun = true;
			SphereShieldMesh->SetVisibility(true);
			ScaleFunc(SphereShieldMesh);
			TLFinish.BindUFunction(this, FName("ShieldOn"));
			ShieldTimeLine->SetTimelineFinishedFunc(TLFinish);
			ShieldTimeLine->PlayFromStart();
			PartCommandFunc(true);
		}
		else
		{
			IsProcessRun = true;
			TLFinish.BindUFunction(this, FName("ShieldProcessOff"));
			ShieldTimeLine->SetTimelineFinishedFunc(TLFinish);
			ShieldTimeLine->ReverseFromEnd();
			PartCommandFunc(false);
		}
	}
}



//"Run check nearest buildings for shield"----------------------------------------------------------------------->
void ATower::PartCommandFunc(bool Up)
{
	if (Up)
	{
		GetWorldTimerManager().SetTimer(Timer3, [this, Up]()
			{
				PSNearestTowers.Empty();
				TArray<AActor*> NearestBuildings;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), NearestBuildings);
				ATower* NearestTower{};
				if (NearestBuildings.Num() > 0)
				{
					for (int i = 0; i < NearestBuildings.Num(); i++)
					{
						if (NearestBuildings[i])
						{
							NearestTower = Cast<ATower>(NearestBuildings[i]);
						}

						if (NearestTower
							&& NearestTower != this
							&& NearestTower->IsProcessRun == false
							//&& GetHorizontalDistanceTo(NearestTower) >= 70.0f
							&& GetHorizontalDistanceTo(NearestTower) <= (CollisionEnergy->GetScaledCapsuleRadius() + NearestTower->TriggerCapsuleInternal->GetUnscaledCapsuleRadius()*0.6f)
							&& NearestTower->SphereShieldMesh->IsVisible() == false
							&& NearestTower->PartShieldMesh->IsVisible() == false)
						{
							NearestTower->PartShieldCreate(GetActorLocation(), Up);
							PSNearestTowers.AddUnique(NearestTower);
						}
					}
				}
				NearestTower = nullptr;
			}, 0.1f, false);
	}
	else
	{
		if (PSNearestTowers.Num() != 0)
		{
			ATower* NearestTower{};
			for (int i = 0; i < PSNearestTowers.Num(); i++)
			{
				if (PSNearestTowers[i])
				{
					NearestTower = Cast<ATower>(PSNearestTowers[i]);
				}
				if (NearestTower
					&& NearestTower != this
					&& NearestTower->IsProcessRun == false
					&& NearestTower->PartShieldMesh->IsVisible() == true)
				{
					NearestTower->PartShieldCreate(GetActorLocation(), Up);
				}
			}
			NearestTower = nullptr;
			PSNearestTowers.Empty();
		}
	}
}



//"Create part of shield functions"------------------------------------------------------------------------------>
void ATower::PartShieldCreate(FVector InitLoc, bool Up)
{
	IsProcessRun = true;
	TlCallbackCreateShield.BindUFunction(this, FName("PartAmountFunc"));
	if (CreateShield)
	{
		PShieldTimeLine->AddInterpFloat(CreateShield, TlCallbackCreateShield);
	}
	if (Up)
	{
		PartShieldMesh->SetVisibility(true);
		ScaleFunc(PartShieldMesh);
		FRotator PSRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InitLoc);
		PartShieldMesh->SetWorldRotation(FRotator(0.0f, PSRotator.Yaw, 0.0f));
		TLFinish.BindUFunction(this, FName("PShieldOn"));
		PShieldTimeLine->SetTimelineFinishedFunc(TLFinish);
		PShieldTimeLine->PlayFromStart();

	}
	else
	{
		TLFinish.BindUFunction(this, FName("PartShieldProcessOff"));
		PShieldTimeLine->SetTimelineFinishedFunc(TLFinish);
		PShieldTimeLine->ReverseFromEnd();
	}
}


void ATower::PartAmountFunc(float Value)
{
	DPartShieldMaterial->SetScalarParameterValue(TEXT("Amount"), Value);
}

void ATower::PShieldOn()
{
	IsProcessRun = false;
}

void ATower::ShieldOn()
{
	IsProcessRun = false;
}

void ATower::PartShieldProcessOff()
{
	PartShieldMesh->SetVisibility(false);
	IsProcessRun = false;
	ScaleFunc(PartShieldMesh);
}

void ATower::AmountFunc(float Value)
{
	DSphereShieldMaterial->SetScalarParameterValue(TEXT("Amount"), Value);
}

void ATower::ShieldProcessOff()
{
	IsProcessRun = false;
	SphereShieldMesh->SetVisibility(false);
	ScaleFunc(SphereShieldMesh);
}

void ATower::ScaleFunc(UStaticMeshComponent* Mesh)
{
	FVector CurrentScale = Mesh->GetRelativeScale3D();
	if (CurrentScale.X > 0.5)
	{
		CurrentScale = CurrentScale / 100.0f;
		Mesh->SetRelativeScale3D(CurrentScale);
	}
	else
	{
		CurrentScale = CurrentScale * 100.0f;
		Mesh->SetRelativeScale3D(CurrentScale);
	}

}




//-----------------------THERE ARE FUNCTIONS FOR DESTROY----------------------------------------//
//"Destroy Tower logic"-------------------------------------------------------------------------->
void ATower::TowerDestroy()
{
	if (Tags.Num() == 0)
	{
		TriggerCapsuleInternal->DestroyComponent();
		TriggerCapsuleExternal->DestroyComponent();
		NetOff();
		Destroy();
	}
	else
	{
		if (Main != this && MainEnemy != this && CanDie)
		{
			CanDie = false;
			if (ActorHasTag(TEXT("Your")))
			{
				TriggerCapsuleInternal->DestroyComponent();
				TriggerCapsuleExternal->DestroyComponent();
				BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator, BoomActor->BuildBoomSystem[0], PActor->YourColor);
				Main->MainFinder(this);
			}
			if (ActorHasTag(TEXT("YourEnemy")))
			{
				TriggerCapsuleInternal->DestroyComponent();
				TriggerCapsuleExternal->DestroyComponent();
				BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator, BoomActor->BuildBoomSystem[0], PActor->EnemyColor);
				MainEnemy->MainFinder(this);
			}
		}
	}
}


//"Reset the parameters"---------------------------------------------------------------------------------------->
void ATower::NetOff()
{
	NiagaraNet->DeactivateImmediate();
	Wave = 0;
	MaxWave = 0;
	Tags.Empty();
	//TriggerCapsuleInternal->ComponentTags.Empty();
	if (IsValid(TriggerCapsuleExternal))
	{
		TriggerCapsuleExternal->ComponentTags.Empty();
	}
	CollisionEnergy->ComponentTags.Empty();
	PartShieldMesh->ComponentTags.Empty();
	SphereShieldMesh->ComponentTags.Empty();
	CollisionEnergy->SetCapsuleHalfHeight(1.0f);
	CollisionEnergy->SetCapsuleRadius(1.0f);
	DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 0.0f);
	DTowMaterial->SetVectorParameterValue(TEXT("LightParam"), PActor->DefaultColor);
	DTowMaterial->SetVectorParameterValue(TEXT("ClickColor"), PActor->DefaultColor);
	DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
	DTowMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), 0.0f);
	IsProcessRun = false;
	DeTouch();
	TW = nullptr;
	AdressTower.Empty();
	if (SphereShieldMesh->IsVisible() == true)
	{
		ShieldCreate();
	}
	if (TemporaryTower && TemporaryTower->ChildTowers.Num() > 0)
	{
		TemporaryTower->ChildTowers.RemoveSwap(this);
		if (TemporaryTower->ChildTowers.Num() == 0)
		{
			if (TemporaryTower == Main)
			{
				Main->NiagaraNet->DeactivateImmediate();
			}
			if (TemporaryTower == MainEnemy)
			{
				MainEnemy->NiagaraNet->DeactivateImmediate();
			}
		}
	}

	TemporaryTower = nullptr;
	FVector CurrentScale = SphereShieldMesh->GetRelativeScale3D();
	if (CurrentScale.X > 1)
	{
		CurrentScale = CurrentScale / 100.0f;
		SphereShieldMesh->SetRelativeScale3D(CurrentScale);
	}
	ChildTowers.Empty();
	if (Name == FName(TEXT("Your")))
	{
		Main->ReloadEnergy(Name);
	}
	if (Name == FName(TEXT("YourEnemy")))
	{
		MainEnemy->ReloadEnergy(Name);
	}
}







//"BeginPlay. It works when game starts"------------------------------------------------------------------->
void ATower::BeginPlay()
{
	Super::BeginPlay();

	PActor = Cast<APreloadActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APreloadActor::StaticClass()));
	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
}




//"Tick. It works every tick, if EverTick = true"---------------------------------------------------------->
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ATower::Destroyed()
{
	Super::Destroyed();

	GetWorldTimerManager().ClearTimer(Timer0);
	//GetWorldTimerManager().ClearTimer(Timer1);
	GetWorldTimerManager().ClearTimer(Timer2);
	GetWorldTimerManager().ClearTimer(Timer3);
	GetWorldTimerManager().ClearTimer(Timer4);


}
