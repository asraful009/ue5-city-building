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
    EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AZMR_PlayerController::Move);
  }
}

void AZMR_PlayerController::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  HandleEdgeScroll(DeltaTime);
}

void AZMR_PlayerController::FocusCameraOnSelected()
{
}

void AZMR_PlayerController::HandleEdgeScroll(const float DeltaTime)
{
  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn = Cast<AZMR_PlayerCameraPawn>(GetPawn());
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

  MoveDir.Normalize();
  CameraPawn->AddActorWorldOffset(MoveDir * EdgeScrollSpeed * DeltaTime, true);
}

void AZMR_PlayerController::Move(const FInputActionValue& Value)
{
  if (Value.GetValueType() != EInputActionValueType::Axis2D)
  {
    return;
  }
  const FVector2D MoveValue = Value.Get<FVector2D>();
  const TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn 
    = Cast<AZMR_PlayerCameraPawn>(GetPawn());
  if (CameraPawn == nullptr || GetWorld() == nullptr)
  {
    return;
  }
  const float Speed = 2000.0f;
  const float DeltaTime = GetWorld()->GetDeltaSeconds();
  
  FVector Forward = CameraPawn->GetActorForwardVector();
  FVector Right   = CameraPawn->GetActorRightVector();
  // Remove vertical tilt
  Forward.Z = 0.f;
  Right.Z   = 0.f;

  Forward.Normalize();
  Right.Normalize();

  FVector MoveDir = Forward * MoveValue.Y + Right * MoveValue.X;
  MoveDir = MoveDir.GetClampedToMaxSize(1.0f);
  FVector Current = CameraPawn->GetActorLocation();
  FVector Target  = Current + MoveDir * Speed * DeltaTime;

  FVector Smooth = FMath::VInterpTo(Current, Target, DeltaTime, 25.0f);

  CameraPawn->SetActorLocation(Smooth);
  
}
