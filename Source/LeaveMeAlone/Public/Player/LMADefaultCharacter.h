//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMAHealthComponent.h"
#include "LMADefaultCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;



UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinCameraDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxCameraDistance = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraZoomSpeed = 100.0f;



		UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

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
	float SprintStaminaCost = 10;

	// Восстановление выносливости
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaRecoveryRate = 10;


private: 
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

protected :
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomCamera(float Value);

	
	// Методы
	void StartSprinting();
	void StopSprinting();
	void UpdateStamina(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
