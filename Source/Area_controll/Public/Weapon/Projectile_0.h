// Copyright (c) Konstantin Pozdeev. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "Projectile_0.generated.h"


class UStaticMeshComponent;
class UMaterialInterface;
class UStaticMesh;
class USphereComponent;
class ABoom;



UCLASS()
class AREA_CONTROLL_API AProjectile_0 : public AProjectile
{
	GENERATED_BODY()
	
public:	

	AProjectile_0();

	virtual void Tick(float DeltaTime) override;


protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UStaticMeshComponent* PMesh;
	UPROPERTY() UStaticMesh* MyStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger") USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh") UMaterialInterface* PMaterial;

	UPROPERTY() FTimerHandle Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables") float InitSpeed;
	UPROPERTY() float Damage;

	UPROPERTY() ABoom* BoomActor;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

};
