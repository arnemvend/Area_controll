// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun2.generated.h"


class UNiagaraComponent;




UCLASS()
class AREA_CONTROLL_API ATGun2 : public AGun
{
	GENERATED_BODY()

public:

	ATGun2();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* NiagaraArrow;

	UFUNCTION() TArray<UPrimitiveComponent*> SetAimsArray();
	UFUNCTION() void SetAim(UPrimitiveComponent* CustomAim);
	UFUNCTION() void SetAuto(bool A);
	UFUNCTION() void OnOffTagret(bool On);

protected:

	UPROPERTY() float ExtRadius;
	UPROPERTY() int ShootEnergyPrice;

	virtual bool ComponentIsFar(UPrimitiveComponent* Component) override;
	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
};
