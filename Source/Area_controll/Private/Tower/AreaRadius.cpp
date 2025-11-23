// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Tower/AreaRadius.h"


AAreaRadius::AAreaRadius()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statick Mesh"));
	AreaMesh->SetupAttachment(RootComponent);
	AreaMesh->SetStaticMesh(LoadObject<UStaticMesh>
		(nullptr, TEXT("StaticMesh'/Game/Buildings/Meshes/Disk.Disk'")));
	AreaMaterial = LoadObject<UMaterialInterface>
	(nullptr, TEXT("Material'/Game/Weapon/FX/MI_AttackRadius.MI_AttackRadius'"));
	AreaMesh->SetMaterial(0, AreaMaterial);
	AreaMesh->SetRelativeScale3D(FVector(20.0f, 20.0f, 1.0f));
	AreaMesh->SetVisibility(false);
}





void AAreaRadius::SetRadius(float R1, float R2, FVector Loc)
{
	if (!(IsValid(AreaMesh) && IsValid(DAreaMaterial)))
	{
		return;
	}

	float Radius1 = FMath::Lerp(0.0f, 0.5f, R1 / 1000.0f);
	float Radius2 = FMath::Lerp(0.0f, 0.5f, R2 / 1000.0f);
	float Num1 = FMath::RoundToFloat(FMath::Lerp(0.0f, StartNumber, R1 / 1000.0f));
	float Num2 = FMath::RoundToFloat(FMath::Lerp(0.0f, StartNumber, R2 / 1000.0f));
	DAreaMaterial->SetScalarParameterValue(TEXT("Radius1Ex"), Radius1);
	DAreaMaterial->SetScalarParameterValue(TEXT("Radius2Ex"), Radius2);
	DAreaMaterial->SetScalarParameterValue(TEXT("NumberLine1"), Num1);
	DAreaMaterial->SetScalarParameterValue(TEXT("NumberLine2"), Num2);
	AreaMesh->SetWorldLocation
		(FVector(Loc.X, Loc.Y, 0.2f), false, nullptr, ETeleportType::ResetPhysics);
	AreaMesh->SetVisibility(true);
}




void AAreaRadius::Stop()
{
	if (IsValid(AreaMesh) && AreaMesh->IsVisible())
	{
		AreaMesh->SetVisibility(false);
	}
}




void AAreaRadius::BeginPlay()
{
	Super::BeginPlay();

	DAreaMaterial = AreaMesh->CreateDynamicMaterialInstance(0, AreaMaterial);
	DAreaMaterial->GetScalarParameterValue(TEXT("NumberLine1"), StartNumber);
}



