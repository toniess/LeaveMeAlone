// LeaveMeAlone Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Components/LMAHealthComponent.h"
#include "DefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class ULMAWeaponComponent;


UCLASS()
class LEAVEMEALONE_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnHealthChanged(float NewHealth);

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
	bool IsInSprintState() const { return bIsSprinting; }

	// Состояние спринта
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting;

	// Выносливость
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float Stamina = 100;

	// Максимальная выносливость
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float MaxStamina = 100;

	// Затраты выносливости на спринт
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintStaminaCost = 20;

	// Восстановление выносливости
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaRecoveryRate = 20;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartSprinting();
	void StopSprinting();
	void UpdateStamina(float DeltaTime);
	void ZoomIn();
	void ZoomOut();

protected:
	UPROPERTY(EditDefaultsOnly)
	float SprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxZoom = 1500.0f;

	UPROPERTY(EditDefaultsOnly)
	float MinZoom = 3000.0f;

	UPROPERTY(EditDefaultsOnly)
	float ZoomStep = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial;

	UPROPERTY()
	UDecalComponent* Cursor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(10.0f, 20.0f, 20.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ComponentsHealth")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

private:
	UFUNCTION()
	void OnDeath();

	void RotationPlayerOnCursor();

private:
	float YRotation = -75.0f;
	float DefaultArmLength = 2000.0f;
	float FOV = 55.0f;
	float CurrentZoom;
};