// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/tProjectile10.h"
#include "Boom.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "Core/AreaControll_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



AtProjectile10::AtProjectile10()
{
	Niagara->SetAsset(LoadObject<UNiagaraSystem>
		(nullptr, TEXT("NiagaraSystem'/Game/Weapon/FX/NI_Lasert10.NI_Lasert10'")));

}





void AtProjectile10::SetDamage()
{
	UAreaControll_GameInstance* GInstance = Cast<UAreaControll_GameInstance>(GetGameInstance());
	if (IsValid(GInstance))
	{
		Damage = GInstance->TG10_Damage;
	}
	GInstance = nullptr;
}





void AtProjectile10::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(OtherComp) || OtherComp->ComponentTags.Num() == 0)
	{
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Ground")))
	{
		Sphere->DestroyComponent();
		BoomActor->CreateBoomFunc
		(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT00BoomSystem[0], FColor::White);
		Niagara->Activate();
		Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
		return;
	}
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
	{
		Sphere->DestroyComponent();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation
		(OtherActor->GetActorLocation(), SweepResult.Location);
		BoomActor->CreateBoomFunc(SweepResult.Location, SpawnRotation, BoomActor->Proj0BoomSystem[0], FColor::White);
		Niagara->Activate();
		Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
		return;
	}
	if (EnemyNames.Num() > 0)
	{
		for (int i = 0; i < EnemyNames.Num(); i++)
		{
			if (IsValid(OtherComp) && OtherComp->ComponentHasTag(EnemyNames[i]))
			{
				Sphere->DestroyComponent();
				UGameplayStatics::ApplyDamage
				(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
				//spown Boom and destroy
				BoomActor->CreateBoomFunc
				(SweepResult.Location, FRotator::ZeroRotator, BoomActor->ProjT10BoomSystem, FColor::White);
				Niagara->Activate();
				Niagara->SetVariablePosition(FName(TEXT("End")), SweepResult.Location);
				return;
			}
		}
	}



}


