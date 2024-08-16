// LeaveMeAlone Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);
DECLARE_MULTICAST_DELEGATE(FOnBulletsEnded);
DECLARE_MULTICAST_DELEGATE(FOnShoot);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();
	void StartFire();
	void EndFire();
	void ChangeClip();
	bool IsCurrentClipEmpty() const;
	bool IsCurrentClipFull() const;
	bool IsClipsAvaliable() const;
	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

public:
	FOnBulletsEnded OnBulletsEnded;
	FOnShoot OnShoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Shoot();
	void DecrementBullets();
	void DrawBulletTrace() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{ 30, 2, false };

	UPROPERTY(EditAnywhere)
	float FireTimerRate = 0.5f;

	FTimerHandle FireTimerHandle;


private:
	FAmmoWeapon CurrentAmmoWeapon;

};
