// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Gun.h"
#include "TGun02.generated.h"


class UStaticMeshComponent;
class UMaterialInterface;



UCLASS()
class AREA_CONTROLL_API ATGun02 : public AGun
{
	GENERATED_BODY()

public:

	ATGun02();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* GunRMesh;

	UPROPERTY() UMaterialInterface* GunRMaterial;

	UPROPERTY() int ShootEnergyPrice;

	virtual void BeginPlay() override;

	virtual void Rotate(float Amount) override;
	virtual void Tracking() override;
	virtual void Fire() override;
};
