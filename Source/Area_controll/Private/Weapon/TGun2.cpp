// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/TGun2.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"





ATGun2::ATGun2()
{
	NiagaraArrow = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraArrowComponent"));
	NiagaraArrow->SetupAttachment(RootComponent);
	NiagaraArrow->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Buildings/FX/NI_Arrow.NI_Arrow'")));
	NiagaraArrow->SetAutoActivate(false);
	

	IsLow = true;
	IsAuto = false;
}





bool ATGun2::ComponentIsFar(UPrimitiveComponent* Component)
{
	const float Dist = HorizontalDistance(GetActorLocation(), Component->GetComponentLocation());
	return (Dist > (GunRadius->GetScaledCapsuleRadius() + 30.0) || Dist <= ExtRadius);
}




void ATGun2::Rotate(float Amount)
{
	Super::Rotate(Amount);

	SetActorRotation(UKismetMathLibrary::RLerp
	(GetActorRotation(), FRotator(0.0f, Rot.Yaw, 0.0f), Amount, true));
}




void ATGun2::Tracking()
{
}





TArray<UPrimitiveComponent*> ATGun2::SetAimsArray()
{
	TArray<UPrimitiveComponent*> ComponentsArr;
	TArray<UPrimitiveComponent*> AimsArr;

	GunRadius->GetOverlappingComponents(ComponentsArr);

	if (ComponentsArr.Num() > 0 && EnemyNames.Num() > 0)
	{
		for (int k = ComponentsArr.Num() - 1; k >= 0; --k)
		{
			if (IsValid(ComponentsArr[k]) && ComponentsArr[k]->ComponentTags.Num() == 0)
			{
				ComponentsArr.RemoveAtSwap(k);
			}
		}
		for (int i = 0; i < ComponentsArr.Num(); i++)
		{
			for (int k = 0; k < EnemyNames.Num(); k++)
			{
				if (ComponentsArr.IsValidIndex(i) &&
				IsValid(ComponentsArr[i]) &&
				ComponentsArr[i]->ComponentHasTag(EnemyNames[k]))
				{
					AimsArr.Add(ComponentsArr[i]);
					break;
				}
			}
		}
	}
	ComponentsArr.Empty();

	if (AimsArr.Num() > 0)
	{
		for (int i = AimComponents.Num() - 1; i >= 0; --i)
		{
			if (!IsValid(AimComponents[i]) || ComponentIsFar(AimComponents[i]))
			{
				AimComponents.RemoveAtSwap(i);
			}
		}
	}

	return AimsArr;
}


//if autoaim is disabled
void ATGun2::SetAim(UPrimitiveComponent* CustomAim)
{
	AimComponents.Empty();
	AimComponent = CustomAim;
	OnOffTagret(true);
	if (!IsFirst)
	{
		PauseAimAndFire();
		GetWorldTimerManager().ClearTimer(TimerFreeRotate);
		FreeRotateTimeLine->Stop();
		StartRotateTimeline();
	}
}


void ATGun2::SetAuto(bool A)
{
	IsAuto = A;
	if (!IsAuto)
	{
		PauseAimAndFire();
		TimerElapsed();
		AimComponent = nullptr;
		AimComponents.Empty();
	}
	else
	{
		Start();
	}
}


void ATGun2::OnOffTagret(bool On)
{
	if (!On)
	{
		NiagaraArrow->DeactivateImmediate();
	}
	else if (IsValid(AimComponent) && !IsAuto)
	{
		FVector Loc = GetActorLocation();
		Loc.Z = 1.0f;
		NiagaraArrow->SetVariablePosition("BeamStart", Loc);
		Loc.X = AimComponent->GetComponentLocation().X;
		Loc.Y = AimComponent->GetComponentLocation().Y;
		NiagaraArrow->SetVariablePosition("BeamEnd", Loc);
		NiagaraArrow->Activate();
	}
}
