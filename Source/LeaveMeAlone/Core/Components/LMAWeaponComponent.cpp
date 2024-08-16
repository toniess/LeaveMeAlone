// LeaveMeAlone Games. All rights reserved.


#include "Core/Components/LMAWeaponComponent.h"
#include "Core/Animations/LMAReloadFinishedAnimNotify.h"
#include "../Weapons/LMABaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();

	InitAnimNotify();
}

// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules,"r_Weapon_Socket");
		}
	}

	Weapon->OnBulletsEnded.AddUObject(this, &ULMAWeaponComponent::OnBulletsEnded);
	Weapon->OnShoot.AddUObject(this, &ULMAWeaponComponent::PlayFireMontage);
}

void ULMAWeaponComponent::Fire()
{
	if (Weapon && !AnimReloading)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("fire canReload = %i"), AnimReloading));
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::InitReloadNotify()
{
	if (!ReloadMontage) return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	InitReloadNotify();
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
		Weapon->ChangeClip();
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && !(Weapon->IsCurrentClipFull()) && Weapon->IsClipsAvaliable();
}

void ULMAWeaponComponent::Reload()
{
	OnBulletsEnded();
}

void ULMAWeaponComponent::StartAutoFire()
{
	if (Weapon && !AnimReloading)
		Weapon->StartFire();
}

void ULMAWeaponComponent::EndAutoFire()
{
	Weapon->EndFire();
}

void ULMAWeaponComponent::OnBulletsEnded()
{
	if (!CanReload()) return;
	Weapon->EndFire();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::PlayFireMontage() const
{
	if (!FireMontage) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(FireMontage);
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon)
const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}