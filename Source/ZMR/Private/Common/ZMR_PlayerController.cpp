// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_PlayerController.h"

#include "Common/ZMR_PlayerCameraPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

AZMR_PlayerController::AZMR_PlayerController()
{
  bShowMouseCursor = true;
  bEnableClickEvents = true;
  bEnableMouseOverEvents = true;
  PrimaryActorTick.bCanEverTick = true;
}

void AZMR_PlayerController::BeginPlay()
{
  Super::BeginPlay();

  FInputModeGameAndUI InputMode;
  InputMode.SetHideCursorDuringCapture(false);
  InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  SetInputMode(InputMode);

  // Get Local Player subsystem
  if (const TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
  {
    if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem
      = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
      // Add your mapping context with priority 0
      if (IMC_CityBuilder) // assign this in the editor
      {
        Subsystem->AddMappingContext(IMC_CityBuilder, 0);
      }
    }
  }
}

void AZMR_PlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();
  if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent
    = Cast<UEnhancedInputComponent>(InputComponent))
  {
    EnhancedInputComponent->BindAction(IA_Move, 
      ETriggerEvent::Triggered, this, &AZMR_PlayerController::Move);
    
    EnhancedInputComponent->BindAction(IA_Zoom, 
      ETriggerEvent::Triggered, this, &AZMR_PlayerController::Zoom);
    
    EnhancedInputComponent->BindAction(IA_Rotate, 
      ETriggerEvent::Triggered, this, &AZMR_PlayerController::RotateCamera);
  }
}

void AZMR_PlayerController::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  // Handle keyboard priority timer
  if (KeyboardMoveTimer > 0.f)
  {
    KeyboardMoveTimer -= DeltaTime;
    bIsKeyboardMoving = true;
  }
  else
  {
    bIsKeyboardMoving = false;
  }

  // Only edge scroll if keyboard NOT active
  if (!bIsKeyboardMoving)
  {
    HandleEdgeScroll(DeltaTime);
  }
}

void AZMR_PlayerController::FocusCameraOnSelected()
{
}

void AZMR_PlayerController::HandleEdgeScroll(const float DeltaTime)
{
  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn = GetPawn<AZMR_PlayerCameraPawn>();
  if (CameraPawn == nullptr)
  {
    return;
  }

  int32 SizeX = 0;
  int32 SizeY = 0;
  GetViewportSize(SizeX, SizeY);
  if (SizeX <= 0 || SizeY <= 0)
  {
    return;
  }

  float MouseX = 0.f;
  float MouseY = 0.f;
  if (!GetMousePosition(MouseX, MouseY))
  {
    return;
  }

  FVector MoveDir = FVector::ZeroVector;

  if (MouseX <= EdgeThreshold)
    MoveDir -= CameraPawn->GetActorRightVector();
  else if (MouseX >= SizeX - EdgeThreshold)
    MoveDir += CameraPawn->GetActorRightVector();

  if (MouseY <= EdgeThreshold)
    MoveDir += CameraPawn->GetActorForwardVector();
  else if (MouseY >= SizeY - EdgeThreshold)
    MoveDir -= CameraPawn->GetActorForwardVector();

  MoveDir.Z = 0.f;
  if (MoveDir.IsNearlyZero())
  {
    return;
  }

  MoveDir = MoveDir.GetSafeNormal();
  FVector Current = CameraPawn->GetActorLocation();
  FVector Target = Current + MoveDir * Speed * DeltaTime;

  FVector Smooth = FMath::VInterpTo(Current, Target, DeltaTime, MoveSmoothSpeed);

  CameraPawn->SetActorLocation(Smooth);
}

void AZMR_PlayerController::Move(const FInputActionValue& Value)
{
  if (Value.GetValueType() != EInputActionValueType::Axis2D)
  {
    return;
  }
  const FVector2D MoveValue = Value.Get<FVector2D>();

  // ✅ EARLY RETURN if no input
  if (MoveValue.IsNearlyZero())
  {
    return;
  }

  // mark keyboard active
  bIsKeyboardMoving = true;
  KeyboardMoveTimer = 0.1f; // buffer time

  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn = GetPawn<AZMR_PlayerCameraPawn>();
  if (CameraPawn == nullptr || GetWorld() == nullptr)
  {
    return;
  }
  const float DeltaTime = GetWorld()->GetDeltaSeconds();
  const FVector Forward = CameraPawn->GetActorForwardVector();
  const FVector Right = CameraPawn->GetActorRightVector();
  const FVector Current = CameraPawn->GetActorLocation();
  FVector MoveDir =
    Forward * MoveValue.Y +
    Right * MoveValue.X;

  MoveDir.Z = 0.f;
  MoveDir = MoveDir.GetSafeNormal();
  MoveDir.Z = 0.f;
  MoveDir = MoveDir.GetSafeNormal();

  const FVector Target = Current + MoveDir * Speed * DeltaTime;

  const FVector Smooth = FMath::VInterpTo(Current, Target, DeltaTime, MoveSmoothSpeed);

  CameraPawn->SetActorLocation(Smooth);
}

void AZMR_PlayerController::Zoom(const FInputActionValue& Value)
{
  const float ZoomValue = Value.Get<float>();
  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn = 
    Cast<AZMR_PlayerCameraPawn>(GetPawn());
  if (!CameraPawn) return;
  CameraPawn->HandleZoom(ZoomValue);
}

void AZMR_PlayerController::RotateCamera(const FInputActionValue& Value)
{
  const FVector2D Input = Value.Get<FVector2D>();
  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn = 
    Cast<AZMR_PlayerCameraPawn>(GetPawn());
  
  if (!CameraPawn || !CameraPawn->IsSpringArmInitialized()) return;
  UE_LOG(LogTemp, Warning, TEXT("Rotate Camera"));
  // Yaw (left/right)
  CameraPawn->AddActorWorldRotation(FRotator(0.f, Input.X, 0.f));
  
  FRotator NewRot = CameraPawn->GetSpringArmRotator();
  NewRot.Pitch = FMath::Clamp(NewRot.Pitch + Input.Y, -80.f, -10.f);
  CameraPawn->SetSpringArmRotator(NewRot);
}
