// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile22.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/MaterialBillboardComponent.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


AtProjectile22::AtProjectile22()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InitialLifeSpan = 8.0f;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(4.0f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	Billboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboard"));
	Billboard->SetupAttachment(Sphere);
	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Buildings/Materials/MI_DLight.MI_DLight'"));
	FMaterialSpriteElement NewElement;
	NewElement.Material = Material;
	Billboard->Elements.Add(NewElement);

	NiagaraStart = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraStart"));
	NiagaraStart->SetupAttachment(RootComponent);
	NiagaraStart->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_ShootT22.NI_ShootT22'")));
	NiagaraStart->SetAutoActivate(true);


	EnemyNames.Empty();
	EnemyNames.Add("WildLighter");
	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("InternalWild");

	DTime = 0.04f;
	GBoost = -3.0f;
	Time = 0.0f;
}





void AtProjectile22::Start()
{
	BoomActor = Cast<ABoom>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoom::StaticClass()));
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG22_Damage;
		Accurary = GInstance->TG22_Accurary;
	}
	GInstance = nullptr;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AtProjectile22::OnOverlapBegin);

	const FVector StartLoc = Sphere->GetComponentLocation();
	Aim = FVector(Aim.X + FMath::FRandRange(-Accurary, Accurary), 
		Aim.Y + FMath::FRandRange(-Accurary, Accurary), 0.0f);

	Sphere->SetWorldRotation(FRotator(Sphere->GetComponentRotation().Pitch,
		UKismetMathLibrary::FindLookAtRotation(StartLoc, Aim).Yaw,
		Sphere->GetComponentRotation().Roll));

	const FVector StartForward = Sphere->GetForwardVector();

	const float Lenght = (FVector2D(StartLoc.X, StartLoc.Y) - FVector2D(Aim.X, Aim.Y)).Length();
	GBoost = FMath::Min(0.0f, GBoost);

	const float ForwardXY = FVector2D(StartForward.X, StartForward.Y).Length();
	const float ForwardZ = StartForward.Z;

	float QuadroSpeed = - (GBoost * Lenght * Lenght / (2 * (ForwardXY * ForwardXY * StartLoc.Z + ForwardXY * ForwardZ * Lenght)));

	QuadroSpeed = FMath::Max(0.0f, QuadroSpeed);

	const float Speed = FMath::Sqrt(QuadroSpeed);

	GetWorldTimerManager().SetTimer(Timer, [this, ForwardZ, Speed, StartLoc, StartForward]()
		{
			if (IsValid(Sphere))
			{
				const float Z = StartLoc.Z + ForwardZ * Speed * Time + GBoost * Time * Time / 2.0f;
				const float X = StartLoc.X + StartForward.X * Speed * Time;
				const float Y = StartLoc.Y + StartForward.Y * Speed * Time;

				Sphere->SetWorldLocation(FVector(X, Y, Z), true);

				Time += DTime;
			}
		}, DTime, true);
}






void AtProjectile22::BeginPlay()
{
	Super::BeginPlay();
}
	




void AtProjectile22::Destroyed()
{
	GetWorldTimerManager().ClearTimer(Timer);

	Super::Destroyed();
}




void AtProjectile22::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT22BoomSystem[0], FColor::White);
		}
		Destroy();
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(OtherActor->GetActorLocation(), SweepResult.Location);
		if (IsValid(BoomActor))
		{
			BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		}
		Destroy();
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				//spown Boom and destroy
				if (IsValid(BoomActor))
				{
					BoomActor->CreateBoomFunc(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT22BoomSystem[1], FColor::White);
				}
				Destroy();
				return;
			}
		}
	}
}



