// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Weapon/Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyNames.Add("Internal");
	EnemyNames.Add("InternalEnemy");
	EnemyNames.Add("NeutralInternal");
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void AProjectile::SetBeginOvelap()
{
}*/

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

