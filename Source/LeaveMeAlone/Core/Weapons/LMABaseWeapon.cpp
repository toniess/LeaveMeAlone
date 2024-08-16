// LeaveMeAlone Games. All rights reserved.

#include "Core/Weapons/LMABaseWeapon.h"
#include "GameFramework/Character.h"


// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	CurrentAmmoWeapon.Clips = AmmoWeapon.Clips;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALMABaseWeapon::Fire()
{
	Shoot();
}

void ALMABaseWeapon::Shoot()
{
	if (!IsCurrentClipEmpty())
	{
		OnShoot.Broadcast();
		DrawBulletTrace();
		DecrementBullets();
	}
}

void ALMABaseWeapon::DrawBulletTrace() const
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
}

void ALMABaseWeapon::ChangeClip()
{
	if (!CurrentAmmoWeapon.Infinite)
		CurrentAmmoWeapon.Clips--;

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::IsClipsAvaliable() const
{
	if (CurrentAmmoWeapon.Infinite == true || CurrentAmmoWeapon.Clips > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	if (CurrentAmmoWeapon.Bullets == 0)
	{
		OnBulletsEnded.Broadcast();
	}
}

void ALMABaseWeapon::StartFire()
{
	if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		Fire();
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ALMABaseWeapon::Fire, FireTimerRate, true);
	}
}

void ALMABaseWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}
