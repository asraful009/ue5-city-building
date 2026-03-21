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
  if (GetPawn() == nullptr)
  {
    UE_LOG(LogTemp, Warning, TEXT("No Player Controller"));
    return;
  } 
  // UE_LOG(LogTemp, Warning, TEXT("CameraPawn: [%s]"), *GetPawn()->GetName());
  TObjectPtr<AZMR_PlayerCameraPawn> CameraPawn 
    = Cast<AZMR_PlayerCameraPawn>(GetPawn());
  if (CameraPawn == nullptr)
  {
    return;
  }
  
  float MouseX, MouseY;
  GetMousePosition(MouseX, MouseY);
  
  // UE_LOG(LogTemp, Warning, TEXT("MouseX: [%f, %f]"), MouseX, MouseY);
  
  int32 SizeX, SizeY;
  GetViewportSize(SizeX, SizeY);
  // UE_LOG(LogTemp, Warning, TEXT("Viewport: [%d, %d]"), SizeX, SizeY);
  
  FVector MoveDir = FVector::ZeroVector;

  if (MouseX <= EdgeThreshold)
    MoveDir -= CameraPawn->GetActorRightVector();
  else if (MouseX >= SizeX - EdgeThreshold)
    MoveDir += CameraPawn->GetActorRightVector();

  if (MouseY <= EdgeThreshold)
    MoveDir += CameraPawn->GetActorForwardVector();
  else if (MouseY >= SizeY - EdgeThreshold)
    MoveDir -= CameraPawn->GetActorForwardVector();

  CameraPawn->AddActorWorldOffset(MoveDir * EdgeScrollSpeed * DeltaTime);
}

void AZMR_PlayerController::Move(const FInputActionValue& Value)
{
  if (Value.GetValueType() != EInputActionValueType::Axis2D)
  {
    return;
  }
  const FVector2D MoveValue = Value.Get<FVector2D>();
  UE_LOG(LogTemp, Warning, TEXT("Input X=%f Y=%f"), MoveValue.X, MoveValue.Y);
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
  UE_LOG(LogTemp, Warning, TEXT("Forward: [%s], Right: [%s]"), *Forward.ToString(), *Right.ToString());
  // Remove vertical tilt
  Forward.Z = 0.f;
  Right.Z   = 0.f;

  Forward.Normalize();
  Right.Normalize();

  const FVector MoveDir = Forward * MoveValue.Y + Right * MoveValue.X;
  CameraPawn->AddActorWorldOffset(MoveDir * Speed * DeltaTime, true);
  
}
