// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_PlayerController.h"

#include "Common/ZMR_PlayerCameraPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Building/ZMR_BuildMenuWidget.h"
#include "Common/ZMR_ObjectSelectInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Walker/ZMR_WalkerBase.h"

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
  CameraPawnRef = GetPawn<AZMR_PlayerCameraPawn>();

  TArray<AActor*> Temp;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZMR_WalkerBase::StaticClass(), Temp);

  for (AActor* Actor : Temp)
  {
     AZMR_WalkerBase* Walker = Cast<AZMR_WalkerBase>(Actor);
     if (Walker)
     {
       LocalWalkerActorRef = Walker;
       break;
     };
  }
  
  auto Menu = CreateWidget<UZMR_BuildMenuWidget>(GetWorld(), BuildMenuClass);
  Menu->AddToViewport();
}                         

void AZMR_PlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();
  if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent
    = Cast<UEnhancedInputComponent>(InputComponent))
  {
    EnhancedInputComponent->BindAction(IA_Move,
                                       ETriggerEvent::Triggered,
                                       this,
                                       &AZMR_PlayerController::Move);

    EnhancedInputComponent->BindAction(IA_Zoom,
                                       ETriggerEvent::Triggered,
                                       this,
                                       &AZMR_PlayerController::Zoom);

    EnhancedInputComponent->BindAction(IA_Rotate,
                                       ETriggerEvent::Triggered,
                                       this,
                                       &AZMR_PlayerController::RotateCamera);

    EnhancedInputComponent->BindAction(IA_MouseHold,
                                       ETriggerEvent::Started,
                                       this,
                                       &AZMR_PlayerController::MouseDragStart);

    EnhancedInputComponent->BindAction(IA_MouseHold,
                                       ETriggerEvent::Completed,
                                       this,
                                       &AZMR_PlayerController::MouseDragStop);

    EnhancedInputComponent->BindAction(IA_MouseDrag,
                                       ETriggerEvent::Triggered,
                                       this,
                                       &AZMR_PlayerController::MouseDragMove);

    EnhancedInputComponent->BindAction(IA_MouseSelect,
                                       ETriggerEvent::Started,
                                       this,
                                       &AZMR_PlayerController::MouseSelectObject);
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
  // Yaw (left/right)
  CameraPawn->AddActorWorldRotation(FRotator(0.f, Input.X, 0.f));

  FRotator NewRot = CameraPawn->GetSpringArmRotator();
  NewRot.Pitch = FMath::Clamp(NewRot.Pitch + Input.Y, -80.f, -10.f);
  CameraPawn->SetSpringArmRotator(NewRot);
}

void AZMR_PlayerController::MouseDragStart(const FInputActionValue& Value)
{
  if (!CameraPawnRef.IsValid())
  {
    return;
  }
  bIsMouseHolding = true;
  GetMousePosition(DragStartMousePos.X, DragStartMousePos.Y);
  DragStartCameraPawnPos = CameraPawnRef->GetActorLocation();
}

void AZMR_PlayerController::MouseDragStop(const FInputActionValue& Value)
{
  bIsMouseHolding = false;
  DragStartMousePos = FVector2D::ZeroVector;
  DragStartCameraPawnPos = FVector::ZeroVector;
}

void AZMR_PlayerController::MouseDragMove(const FInputActionValue& Value)
{
  if (!bIsMouseHolding || !CameraPawnRef.IsValid()) return;

  const FVector2D MouseDelta = Value.Get<FVector2D>();
  if (MouseDelta.IsNearlyZero()) return;

  FVector2D CurrentMousePos;
  if (!GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y)) return;
  FVector2D DeltaPos = CurrentMousePos - DragStartMousePos;

  FVector MoveTo = (-CameraPawnRef->GetActorRightVector() * DeltaPos.X +
    CameraPawnRef->GetActorForwardVector() * DeltaPos.Y) * DragSpeed;

  CameraPawnRef->SetActorLocation(DragStartCameraPawnPos + MoveTo);
}

void AZMR_PlayerController::MouseSelectObject()
{
  FHitResult Hit;
  bool bHit = 
    GetHitResultUnderCursor(
      ECC_Visibility, 
      false, 
      Hit);
  if (!bHit)
  {
    MouseDeSelectedObject();
    return;
  }
  if (const TObjectPtr<AActor> HitActor = Hit.GetActor())
  {
    MouseSelectedObject(HitActor); // 👈 THIS is what you missed
  }
  else
  {
    MouseDeSelectedObject();
  }
}

void AZMR_PlayerController::MouseSelectedObject(const TObjectPtr<AActor> NewSelectedObject)
{
  if (NewSelectedObject == MouseSelectedActorRef.Get())
  {
    return;
  }
  if (MouseSelectedActorRef.IsValid() 
    && MouseSelectedActorRef->Implements<UZMR_ObjectSelectInterface>())
  {
    IZMR_ObjectSelectInterface::Execute_OnDeselected(MouseSelectedActorRef.Get());
    MouseSelectedActorRef.Reset();
  }
  
  if (NewSelectedObject 
    && NewSelectedObject->Implements<UZMR_ObjectSelectInterface>())
  {
    MouseSelectedActorRef = NewSelectedObject;
    IZMR_ObjectSelectInterface::Execute_OnSelected(MouseSelectedActorRef.Get());
    if (LocalWalkerActorRef.IsValid())
    {
      LocalWalkerActorRef->SetTargetBuilding(MouseSelectedActorRef.Get());
    }
  }
}

void AZMR_PlayerController::MouseDeSelectedObject()
{
  if (MouseSelectedActorRef.IsValid() 
    && MouseSelectedActorRef->Implements<UZMR_ObjectSelectInterface>())
  {
    IZMR_ObjectSelectInterface::Execute_OnDeselected(MouseSelectedActorRef.Get());
    MouseSelectedActorRef.Reset();
  }
}
