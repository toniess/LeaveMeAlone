//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

private: 
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

protected :
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomCamera(float Value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
