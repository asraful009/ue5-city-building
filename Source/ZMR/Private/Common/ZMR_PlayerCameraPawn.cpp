// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_PlayerCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AZMR_PlayerCameraPawn::AZMR_PlayerCameraPawn()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

  SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
  SpringArm->SetupAttachment(RootComponent);
  SpringArm->TargetArmLength = 1200.f;
  SpringArm->bDoCollisionTest = false;
  SpringArm->bUsePawnControlRotation = false;
  
  // Top-down angle
  SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
  // Camera
  Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

  Camera->bUsePawnControlRotation = false;
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZMR_PlayerCameraPawn::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AZMR_PlayerCameraPawn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZMR_PlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZMR_PlayerCameraPawn::HandleZoom(float Value)
{
  if (!SpringArm) return;
  float NewLength = SpringArm->TargetArmLength - (Value * ZoomSpeed);

  NewLength = FMath::Clamp(NewLength, MinZoom, MaxZoom);

  SpringArm->TargetArmLength = FMath::FInterpTo(
    SpringArm->TargetArmLength,
    NewLength,
    GetWorld()->GetDeltaSeconds(),
    10.f
  );
}

bool AZMR_PlayerCameraPawn::IsSpringArmInitialized() const
{
  if (!SpringArm)
  {
    return false;
  }
  return true;
}

FRotator AZMR_PlayerCameraPawn::GetSpringArmRotator() const
{
  if (!SpringArm)
  {
    return FRotator();
  }
  return SpringArm->GetRelativeRotation();
}

void AZMR_PlayerCameraPawn::SetSpringArmRotator(const FRotator NewRotator) const
{
  if (!SpringArm)
  {
    return;
  }
  SpringArm->SetRelativeRotation(NewRotator);
}

void AZMR_PlayerCameraPawn::MoveForward(float Value)
{
}

void AZMR_PlayerCameraPawn::MoveRight(float Value)
{
}

