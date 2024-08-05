//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");

	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = ArmLength;
	SpringArm->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(FOV);
	Camera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();	

	if (IsValid(CursorMaterial))
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStamina(DeltaTime);

	// пока что отключаем дурацкое вращение камеры

	/*
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}

	}
	*/

}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &ALMADefaultCharacter::ZoomCamera);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprinting);
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::ZoomCamera(float Value) 
{
	if (Value != 0.0f)
	{
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * CameraZoomSpeed, MinCameraDistance, MaxCameraDistance);
	}
}

void ALMADefaultCharacter::StartSprinting()
{
	if (Stamina > 0)
	{
		bIsSprinting = true;
	}
}

void ALMADefaultCharacter::StopSprinting()
{
	bIsSprinting = false;
}

void ALMADefaultCharacter::UpdateStamina(float DeltaTime)
{
	if (bIsSprinting)
	{
		Stamina -= SprintStaminaCost * DeltaTime;
		if (Stamina < 0)
			Stamina = 0;
	}
	else
	{
		Stamina += StaminaRecoveryRate * DeltaTime;
		if (Stamina > MaxStamina)
			Stamina = MaxStamina;
	}

	UE_LOG(LogTemp, Display, TEXT("stamina %f"), Stamina);
	UE_LOG(LogTemp, Display, TEXT("isSprint %i"), bIsSprinting);
}