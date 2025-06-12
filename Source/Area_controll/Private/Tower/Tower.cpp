// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/Tower.h"
#include "Boom.h"
#include "Weapon/Gun.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Core/AreaControll_GameMode.h"
#include "Core/AreaControll_PlayerController.h"
#include "HUD/TowerWidget.h"
#include "Interfaces/GameWidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tower/MainTower.h"


ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	UpGun = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpGun"));
	UpGun->SetupAttachment(RootComponent);

	MidGun = CreateDefaultSubobject<UChildActorComponent>(TEXT("MidGun"));
	MidGun->SetupAttachment(RootComponent);

	LowGun = CreateDefaultSubobject<UChildActorComponent>(TEXT("LowGun"));
	LowGun->SetupAttachment(RootComponent);


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


	//last update for repeater
	MyStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Repeater.Repeater'"));



	//"Set parameters of triggers"------------------------------------------------------------------------------->
	//Internal trigger
	TriggerCapsuleInternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InternalCollisionComponent"));
	TriggerCapsuleInternal->SetupAttachment(RootComponent);
	TriggerCapsuleInternal->SetCapsuleRadius(28.0f);
	TriggerCapsuleInternal->SetCapsuleHalfHeight(70.0f);
	TriggerCapsuleInternal->ComponentTags.Add(TEXT("NeutralInternal"));

	//External trigger
	TriggerCapsuleExternal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ExternalCollisionComponent"));
	TriggerCapsuleExternal->SetupAttachment(RootComponent);
	TriggerCapsuleExternal->SetCapsuleRadius(200.0f);
	TriggerCapsuleExternal->SetCapsuleHalfHeight(300.0f);
	TriggerCapsuleExternal->ComponentTags.Add(TEXT("NeutralExternal"));

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
	NiagaraNet->SetAutoActivate(false);
	


	NiagaraRepeater = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraRepeaterComponent"));
	NiagaraRepeater->SetupAttachment(RootComponent);
	NiagaraNetSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Repeater.NI_Repeater'"));
	NiagaraRepeater->SetAsset(NiagaraNetSystem);
	NiagaraRepeater->SetAutoActivate(false);



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
	NameMain = FName(TEXT("Main"));
	NameMainEnemy = FName(TEXT("MainEnemy"));
	IsProcessRun = false;
	IsClicked = false;
	IsRepeater = false;
	CanDie = true;
	CanDamage = false;


	OnTakeAnyDamage.AddDynamic(this, &ATower::OnTakeDamage);
	OnInputTouchBegin.AddDynamic(this, &ATower::Touch);
}



void ATower::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
                          AActor* DamageCauser)
{
	if (CanDamage && Health > 0)
	{
		Health = Health - Damage;
		SetDamageFunc();
	}
}



//"Change colors"------------------------------------------------------------------------------------------------>
void ATower::ColorsFunc(FColor Color)
{
	DDiskMaterial->SetVectorParameterValue(TEXT("Color_Param"), Color);
	if (!IsRepeater)
	{
		DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 1.0f);
	}

	DTowMaterial->SetVectorParameterValue(TEXT("LightParam"), Color);
	DTowMaterial->SetVectorParameterValue(TEXT("ClickColor"), Color);
	DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
	
	DTowMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), CurrentEmissive);

	DSphereShieldMaterial->SetVectorParameterValue(TEXT("BaseColor"), Color);
}



void ATower::SetDamageFunc()
{
	if (Health <= 0)
	{
		TowerDestroy();
		return;
	}
	float const Alpha = FMath::Clamp(Health / MaxHealth, 0.1f, 1.0f);
	if (DTowMaterial)
	{
		DTowMaterial->SetScalarParameterValue(TEXT("Damage"), Alpha);
	}

	if (!IsValid(UpGun) || !IsValid(MidGun) || !IsValid(LowGun))
	{
		return;
	}

	TArray<UChildActorComponent*> TGuns;
	TGuns.Add(UpGun);
	TGuns.Add(MidGun);
	TGuns.Add(LowGun);

	AGun* MyGun;

	for (int i = 0; i < TGuns.Num(); i++)
	{
		if (IsValid(TGuns[i]) && IsValid(TGuns[i]->GetChildActor()))
		{
			MyGun = Cast<AGun>(TGuns[i]->GetChildActor());
			if (IsValid(MyGun))
			{
				MyGun->DMaterial->SetScalarParameterValue(TEXT("Damage"), Alpha);
			}
		}

	}

	TGuns.Empty(0);
	MyGun = nullptr;
}




//"Functions fot touch actor"------------------------------------------------------------------------------------>
//the function of removing the selection
void ATower::DeTouch(int Value)
{
	if (IsClicked)
	{
		DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
		float Op; 
		DDiskMaterial->GetScalarParameterValue(TEXT("EmissiveMultiply"), Op);
		DDiskMaterial->SetScalarParameterValue(TEXT("EmissiveMultiply"), Op / 2);
		if (IsValid(TW) && TW->IsInViewport())
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
			ATower::DeTouch(0);
		}
		else
		{
			//search for a highlighted tower and deselect it
			if (GMode->PlayerTowers.Num() > 0)
			{
				for (int i = 0; i < GMode->PlayerTowers.Num(); i++)
				{
					if (IsValid(GMode->PlayerTowers[i]) && GMode->PlayerTowers[i] != this
						&& GMode->PlayerTowers[i]->IsClicked)
					{
						GMode->PlayerTowers[i]->DeTouch(0);
					}
				}
			}

			DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 0.0f);
			float Op;
			DDiskMaterial->GetScalarParameterValue(TEXT("EmissiveMultiply"), Op);
			DDiskMaterial->SetScalarParameterValue(TEXT("EmissiveMultiply"), Op * 2);
			IsClicked = true;
			//if the tower has its own, the menu is turned on
			if (ActorHasTag(FName(TEXT("Your"))))
			{
				if (IsValid(TW) && TW->IsInViewport() == false)
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
	if(!IsProcessRun)
	{
		IsRepeater = true;
		if (SphereShieldMesh->IsVisible())
		{
			ShieldCreate();
		}
		TriggerCapsuleExternal->SetCapsuleRadius(TriggerCapsuleExternal->GetScaledCapsuleRadius() * 2.0f);
		TriggerCapsuleExternal->SetCapsuleHalfHeight(TriggerCapsuleExternal->GetScaledCapsuleHalfHeight() * 2.0f);
		CollisionEnergy->SetCapsuleRadius(1.0f);
		CollisionEnergy->SetCapsuleHalfHeight(1.0f);
		DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 0.0f);
		TowerMesh->SetStaticMesh(MyStaticMesh);
		Influence = TriggerCapsuleInternal->GetScaledCapsuleRadius() + TriggerCapsuleExternal->GetScaledCapsuleRadius();
		
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

		NiagaraRepeater->Activate();
		NiagaraRepeater->SetVariableLinearColor(TEXT("Color"), MyColor);
		NiagaraRepeater->SetAutoDestroy(true);
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

	CanDamage = true;
}


//"Function is setter Main and MainEnemy Actors"----------------------------------------------------------------->
void ATower::IsMainFunc(bool IsYour)
{
	if (GMode->PlayerTowers.Num() > 0)
	{
		Main = Cast<AMainTower>(GMode->PlayerTowers[0]);
	}
	if (GMode->EnemyTowers.Num() > 0)
	{
		MainEnemy = Cast<AMainTower>(GMode->PlayerTowers[0]);
	}
	//network search
	if (IsYour)
	{
		ReEnter(Main->Name);
	}
	else
	{
		ReEnter(MainEnemy->Name);
	}
}


//"This function finds Towers in Network"----------------------------------------------------------------------->
void ATower::ReEnter(FName AName)
{
	StepTowers.Empty();
	ChildTowers.Empty();
	TArray<ATower*> ActorsOfClass;
	if (AName == FName(TEXT("Your")))
	{
		ActorsOfClass = GMode->PlayerTowers;
	}
	if (AName == FName(TEXT("YourEnemy")))
	{
		ActorsOfClass = GMode->EnemyTowers;
	}

	//set arrow of towers with tag "Name" in the area of influence
	if (ActorsOfClass.Num() > 0)
	{
		for (int i = 0; i < ActorsOfClass.Num(); i++)
		{
			if (IsValid(ActorsOfClass[i]) && ActorsOfClass[i] != this
				&& (GetHorizontalDistanceTo(ActorsOfClass[i]) <= ActorsOfClass[i]->Influence 
				|| GetHorizontalDistanceTo(ActorsOfClass[i]) <= Influence))
			{
				StepTowers.AddUnique(ActorsOfClass[i]);
			}
		}
	}

	ActorsOfClass.Empty();
	if (StepTowers.Num() == 0)
	{
		if (AName == FName(TEXT("Your")))
		{
			ActorsOfClass = GMode->EnemyTowers;
		}
		if (AName == FName(TEXT("YourEnemy")))
		{
			ActorsOfClass = GMode->PlayerTowers;
		}
	}

	if (ActorsOfClass.Num() > 0)
	{
		for (int i = 0; i < ActorsOfClass.Num(); i++)
		{
			if (IsValid(ActorsOfClass[i]) && ActorsOfClass[i] != this
				&& (GetHorizontalDistanceTo(ActorsOfClass[i]) <= ActorsOfClass[i]->Influence
					|| GetHorizontalDistanceTo(ActorsOfClass[i]) <= Influence))
			{
				StepTowers.AddUnique(ActorsOfClass[i]);
			}
		}
	}

	ActorsOfClass.Empty(0);
	//check distance to other tower
	CheckStep(MaxWave);

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
			if (IsValid(StepTowers[i]) && GetHorizontalDistanceTo(StepTowers[i]) < Distance)
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
	if (IsValid(TemporaryTower))
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
	if (IsValid(TemporaryTower))
	{
		int MaxIndex = 0;
		Name = TemporaryTower->Name;
		MyColor = TemporaryTower->MyColor;
		if (IsValid(TemporaryTower->TW))
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
				if (IsValid(TemporaryTower->ChildTowers[i]) && MaxIndex != TemporaryTower->ChildTowers[i]->AdressTower.Last())
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
				Main->NetStart(Main->MyColor);
			}
			if (TemporaryTower == MainEnemy)
			{
				MainEnemy->NetStart(Main->MyColor);
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
		NiagaraNet->SetVariableLinearColor(FName(TEXT("NetColor")), MyColor);
		ColorsFunc(MyColor);
		GMode->DisabledTowers.RemoveSwap(this);
		if (IsRepeater == false)
		{
			CollisionEnergy->ComponentTags.AddUnique(Name);
			CollisionEnergy->SetCapsuleRadius(100.0f);
			CollisionEnergy->SetCapsuleHalfHeight(100.0f);
			CanFire(true, false, true);
		}
		if (Name == FName(TEXT("Your")))
		{
			GMode->PlayerTowers.AddUnique(this);
			Main->NotNet = 0;
			Main->ReFinder(Main->Name);
			Main->CheckNew(this);

		}
		if (Name == FName(TEXT("YourEnemy")))
		{
			GMode->EnemyTowers.AddUnique(this);
			MainEnemy->NotNet = 0;
			MainEnemy->ReFinder(MainEnemy->Name);
			MainEnemy->CheckNew(this);
		}
	}
	
	
	
}









//-----------------------THERE ARE FUNCTIONS FOR ENERGY SHIELD--------------------------------------------------//

void ATower::CheckShieldansRun(bool IsPlayer, bool ShieldIsOn)
{
	if (ActorHasTag(TEXT("Your")) == IsPlayer && SphereShieldMesh->IsVisible() == ShieldIsOn)
	{
		ShieldCreate();
	}
}





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
		if (!SphereShieldMesh->IsVisible())
		{
			if (PartShieldMesh->GetVisibleFlag())
			{
				PartShieldMesh->SetVisibility(false);
				ScaleFunc(PartShieldMesh);
			}
			//DPartShieldMaterial->SetVectorParameterValue(TEXT("BaseColor"), MyColor);
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
			SphereShieldMesh->ComponentTags.Empty();
			CanDamage = true;
			CanFire(true, false, false);
		}
	}
}



//"Run check nearest buildings for shield"----------------------------------------------------------------------->
void ATower::PartCommandFunc(bool Up)
{
	if (Up)
	{
		GetWorldTimerManager().SetTimer(Timer1, [this, Up]()
			{
				PSNearestTowers.Empty();
				TArray<ATower*> NearestBuildings = GMode->PlayerTowers;
				NearestBuildings += GMode->EnemyTowers;

				if (NearestBuildings.Num() > 0)
				{
					for (int i = 0; i < NearestBuildings.Num(); i++)
					{
						if (IsValid(NearestBuildings[i])
							&& NearestBuildings[i] != this
							&& !NearestBuildings[i]->IsProcessRun
							&& GetHorizontalDistanceTo(NearestBuildings[i]) <= (CollisionEnergy->GetScaledCapsuleRadius()
								+ NearestBuildings[i]->TriggerCapsuleInternal->GetUnscaledCapsuleRadius()*0.6f)
							&& !NearestBuildings[i]->SphereShieldMesh->IsVisible()
							&& !NearestBuildings[i]->PartShieldMesh->IsVisible())
						{
							NearestBuildings[i]->DPartShieldMaterial->SetVectorParameterValue(TEXT("BaseColor"), MyColor);
							NearestBuildings[i]->PartShieldCreate(GetActorLocation(), Up);
							PSNearestTowers.AddUnique(NearestBuildings[i]);
						}
					}
				}
				NearestBuildings.Empty(0);
			}, 0.1f, false);
	}
	else
	{
		if (PSNearestTowers.Num() != 0)
		{
			for (int i = 0; i < PSNearestTowers.Num(); i++)
			{
				if (IsValid(PSNearestTowers[i])
					&& PSNearestTowers[i] != this
					&& !PSNearestTowers[i]->IsProcessRun
					&& PSNearestTowers[i]->PartShieldMesh->IsVisible())
				{
					PSNearestTowers[i]->PartShieldCreate(GetActorLocation(), Up);
				}
			}
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
		PartShieldMesh->ComponentTags.Empty();
		CanDamage = true;
		CanFire(true, false, false);
	}
}


void ATower::PartAmountFunc(float Value)
{
	DPartShieldMaterial->SetScalarParameterValue(TEXT("Amount"), Value);
}

void ATower::PShieldOn()
{
	IsProcessRun = false;
	PartShieldMesh->ComponentTags.AddUnique(TEXT("Shield"));
	CanDamage = false;
	CanFire(false, false, false);
}

void ATower::ShieldOn()
{
	IsProcessRun = false;
	SphereShieldMesh->ComponentTags.AddUnique(TEXT("Shield"));
	CanDamage = false;
	CanFire(false, false, false);
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


void ATower::CanFire(bool CanFire, bool bNeedStop, bool bNeedStart)
{
	if (!IsValid(UpGun) || !IsValid(MidGun) || !IsValid(LowGun))
	{
		return;
	}

	TArray<UChildActorComponent*> TGuns;
	TGuns.Add(UpGun);
	TGuns.Add(MidGun);
	TGuns.Add(LowGun);

	AGun* MyGun;

	for (int i = 0; i < TGuns.Num(); i++)
	{
		if (IsValid(TGuns[i]) && IsValid(TGuns[i]->GetChildActor()))
		{
			MyGun = Cast<AGun>(TGuns[i]->GetChildActor());
			if (IsValid(MyGun))
			{
				MyGun->IsShieldOn = !CanFire;
				if (bNeedStop && IsValid(MyGun->DMaterial) && IsValid(GInstance))
				{
					MyGun->Stop();
					MyGun->DMaterial->SetVectorParameterValue(TEXT("LightParam"), GInstance->DefaultColor);
					MyGun->DMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), 0.0f);
				}
				if (bNeedStart)
				{
					MyGun->SpawnFunc(MyColor);
				}
			}
		}

	}

	TGuns.Empty(0);
	MyGun = nullptr;
}




//-----------------------THERE ARE FUNCTIONS FOR DESTROY----------------------------------------//
//"Destroy Tower logic"-------------------------------------------------------------------------->
void ATower::TowerDestroy()
{
	if (!IsValid(BoomActor))
	{
		return;
	}
	if (Tags.Num() == 0)
	{
		TriggerCapsuleExternal->DestroyComponent();
		TriggerCapsuleInternal->DestroyComponent();
		BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator,
			BoomActor->BuildBoomSystem[0], FColor::White);
		NetOff();
		GMode->DisabledTowers.RemoveSwap(this);
		Destroy();
	}
	else
	{
		if (Main != this && MainEnemy != this && CanDie)
		{
			BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator,
				BoomActor->BuildBoomSystem[0], MyColor);
			TowerDelete();
		}
	}
}




void ATower::TowerDelete()
{
	TriggerCapsuleExternal->DestroyComponent();
	TriggerCapsuleInternal->DestroyComponent();
	CanDie = false;
	if (ActorHasTag(TEXT("Your")))
	{
		Main->MainFinder(this);
	}
	if (ActorHasTag(TEXT("YourEnemy")))
	{
		MainEnemy->MainFinder(this);
	}
}




void ATower::RunDelete()
{
	DeTouch(0);
	NiagaraRepeater->DeactivateImmediate();
	NiagaraRepeater->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Builder.NI_Builder'")));
	NiagaraRepeater->Activate();
	if (IsValid(BoomActor))
	{
		BoomActor->CreateBoomFunc(GetActorLocation(), FRotator::ZeroRotator,
			BoomActor->BuildBoomSystem[1], MyColor);
	}
	GetWorldTimerManager().SetTimer(Timer2, [this]()
		{
			TowerDelete();
		}, 0.5f, false);
}


//"Reset the parameters"---------------------------------------------------------------------------------------->
void ATower::NetOff()
{
	NiagaraNet->DeactivateImmediate();
	Wave = 0;
	MaxWave = 0;
	Tags.Empty();
	if (IsValid(TriggerCapsuleInternal))
	{
		TriggerCapsuleInternal->ComponentTags.Empty();
		TriggerCapsuleInternal->ComponentTags.Add(TEXT("NeutralInternal"));
	}
	if (IsValid(TriggerCapsuleExternal))
	{
		TriggerCapsuleExternal->ComponentTags.Empty();
		TriggerCapsuleExternal->ComponentTags.Add(TEXT("NeutralExternal"));
	}
	CollisionEnergy->ComponentTags.Empty();
	//PartShieldMesh->ComponentTags.Empty();
	//SphereShieldMesh->ComponentTags.Empty();
	CollisionEnergy->SetCapsuleHalfHeight(1.0f);
	CollisionEnergy->SetCapsuleRadius(1.0f);
	DDiskMaterial->SetScalarParameterValue(TEXT("ShadowParam"), 0.0f);
	if (IsValid(GInstance))
	{
		DTowMaterial->SetVectorParameterValue(TEXT("LightParam"), GInstance->DefaultColor);
		DTowMaterial->SetVectorParameterValue(TEXT("ClickColor"), GInstance->DefaultColor);
	}
	DTowMaterial->SetScalarParameterValue(TEXT("ClickValue"), 1.0f);
	DTowMaterial->SetScalarParameterValue(TEXT("Param_EmissiveMultiply"), 0.0f);
	IsProcessRun = false;
	DeTouch(0);
	TW = nullptr;
	AdressTower.Empty();
	if (SphereShieldMesh->IsVisible())
	{
		ShieldCreate();
	}
	CanDamage = true;
	CanFire(false, true, false);
	if (IsValid(TemporaryTower) && TemporaryTower->ChildTowers.Num() > 0)
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
	/*FVector CurrentScale = SphereShieldMesh->GetRelativeScale3D();
	if (CurrentScale.X > 1)
	{
		CurrentScale = CurrentScale / 100.0f;
		SphereShieldMesh->SetRelativeScale3D(CurrentScale);
	}*/
	ChildTowers.Empty();
	GMode->DisabledTowers.AddUnique(this);
	if (Name == FName(TEXT("Your")))
	{
		GMode->PlayerTowers.RemoveSwap(this);
		Main->ReloadEnergy(Name);
	}
	if (Name == FName(TEXT("YourEnemy")))
	{
		GMode->EnemyTowers.RemoveSwap(this);
		MainEnemy->ReloadEnergy(Name);
	}
}





//-----------------------TOWERGUN LOGIC--------------------------------------------//
//Create TowerGun------------------------------------------------------------------->
void ATower::CreateGun(int Type, int Number)
{
	if (!IsValid(UpGun) || !IsValid(MidGun) || !IsValid(LowGun) || !IsValid(GInstance))
	{
		return;
	}

	TSubclassOf<AGun> Gun;
	if (Name == FName(TEXT("Your")))
	{
		Gun = GInstance->TGuns[Type * 5 + Number];
	}
	else if (Name == FName(TEXT("YourEnemy")))
	{
		//in future
	}
	else
	{
		return;
	}

	if (!Gun)
	{
		return;
	}

	UChildActorComponent* TargetGun;

	switch (Type)
	{
	case 0: TargetGun = UpGun; break;
	case 1: TargetGun = MidGun; break;
	case 2: TargetGun = LowGun; break;
	default: return;
	}

	TargetGun->DestroyChildActor();
	TargetGun->SetChildActorClass(Gun);
	TargetGun->CreateChildActor();
	if (IsValid(TargetGun) && IsValid(TargetGun->GetChildActor()))
	{
		AGun* MyGun = Cast<AGun>(TargetGun->GetChildActor());
		if (IsValid(MyGun))
		{
			if (SphereShieldMesh->IsVisible() || PartShieldMesh->IsVisible())
			{
				MyGun->IsShieldOn = true;
			}
			MyGun->SpawnFunc(MyColor);
			SetDamageFunc();
			MyGun = nullptr;
		}
	}
	TargetGun = nullptr;
	Gun = nullptr;
}



void ATower::DeleteGun(int Type)
{
	UChildActorComponent* TargetGun;

	switch (Type)
	{
	case 0: TargetGun = UpGun; break;
	case 1: TargetGun = MidGun; break;
	case 2: TargetGun = LowGun; break;
	default: return;
	}

	if (IsValid(TargetGun) && IsValid(TargetGun->GetChildActor()))
	{
		AGun* MyGun = Cast<AGun>(TargetGun->GetChildActor());
		MyGun->DeleteFunc();
		MyGun = nullptr;
	}
	TargetGun = nullptr;
}







//"BeginPlay. It works when game starts"------------------------------------------------------------------->
void ATower::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	GMode = Cast<AAreaControll_GameMode>(GetWorld()->GetAuthGameMode());
	PController = Cast<AAreaControll_PlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(GInstance))
	{
		EnergyLoss = GInstance->ShieldEnergyLoss;
		Repear = GInstance->Repear;
		EnergyRepear = GInstance->EnergyRepear;
	}

	PController->FindReferences();
	GMode->DisabledTowers.AddUnique(this);

	GetWorldTimerManager().SetTimer(Timer0, [this]()
		{
			TArray<UUserWidget*> UWidgets;
			TArray<AActor*> Actors;
			IGameWidgetInterface* GameWidgetInterface;
			UWidgetBlueprintLibrary::GetAllWidgetsWithInterface
			(this, UWidgets, UGameWidgetInterface::StaticClass(), true);
			UGameplayStatics::GetAllActorsWithInterface(this, UGameWidgetInterface::StaticClass(), Actors);
			if (UWidgets.Num() > 0)
			{
				GameWidgetInterface = Cast<IGameWidgetInterface>(UWidgets[0]);
				if (GameWidgetInterface)
				{
					GameWidgetInterface->GetOnDeTouchDelegate().AddDynamic(this, &ATower::DeTouch);
					GameWidgetInterface->GetOnShieldDelegate().AddDynamic(this, &ATower::CheckShieldansRun);
				}
				GameWidgetInterface = nullptr;
			}
			if (Actors.Num() > 0)
			{
				GameWidgetInterface = Cast<IGameWidgetInterface>(Actors[0]);
				if (GameWidgetInterface)
				{
					GameWidgetInterface->GetOnDeTouchDelegate().AddDynamic(this, &ATower::DeTouch);
				}
				GameWidgetInterface = nullptr;
			}
			UWidgets.Empty();
			Actors.Empty();
		}, 0.1f, false);

	GetWorldTimerManager().SetTimer(Timer3, [this]()
		{
		    if (SphereShieldMesh->IsVisible())
		    {
				int a = 0;
				if (Name == FName(TEXT("Your")))
				{
					GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - EnergyLoss);
					a = GMode->PlayerEnergy;
				}
				if (Name == FName(TEXT("YourEnemy")))
				{
					GMode->EnemyEnergy = FMath::Max(0, GMode->EnemyEnergy - EnergyLoss);
					a = GMode->EnemyEnergy;
				}
				if (a < 10)
				{
					ShieldCreate();
				}
		    }
		    if (Health < MaxHealth && Tags.Num() > 0)
		    {
				if (Name == FName(TEXT("Your")) && GMode->PlayerEnergy >= EnergyRepear)
				{
					GMode->PlayerEnergy = FMath::Max(0, GMode->PlayerEnergy - EnergyRepear);
					Health = FMath::Min(Health + Repear, MaxHealth);
					SetDamageFunc();
				}
				if (Name == FName(TEXT("YourEnemy")))
				{
					GMode->EnemyEnergy = FMath::Max(0, GMode->EnemyEnergy - EnergyRepear);
					Health = FMath::Min(Health + Repear, MaxHealth);
					SetDamageFunc();
				}
		    }
		}, 0.5f, true, 0.0f);
}






void ATower::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer0);
	GetWorldTimerManager().ClearTimer(Timer1);
	GetWorldTimerManager().ClearTimer(Timer2);
	GetWorldTimerManager().ClearTimer(Timer3);

	//Timers in Main
	GetWorldTimerManager().ClearTimer(Timer010);
	GetWorldTimerManager().ClearTimer(Timer011);
	GetWorldTimerManager().ClearTimer(Timer012);
	
	Super::Destroyed();
}
