// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::Fire()
{
	Shoot();
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ALMABaseWeapon::Shoot, FireRate, true);
}


void ALMABaseWeapon::StopFire()
{
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	}
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Shoot()
{
	if (IsCurrentClipEmpty())
	{
		StopFire();
		return;
	}

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

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	if (IsCurrentClipEmpty())
	{
		OnClipEmpty.Broadcast();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets==0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::CanReload() const
{
	return !IsCurrentClipFull() && (CurrentAmmoWeapon.Clips > 0 || CurrentAmmoWeapon.Infinite);
}

void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	CurrentAmmoWeapon.Clips--;
}

void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}