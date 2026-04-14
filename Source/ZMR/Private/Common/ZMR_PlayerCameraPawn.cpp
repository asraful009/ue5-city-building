// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_PlayerCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AZMR_PlayerCameraPawn::AZMR_PlayerCameraPawn()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

  // RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

  // SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
  // SpringArm->SetupAttachment(RootComponent);
  // SpringArm->TargetArmLength = 800.f;
  // SpringArm->bEnableCameraLag = true;
  //
  // Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  // Camera->SetupAttachment(SpringArm);
  //
  // Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
  // Movement->MaxSpeed = 1200.f;
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

void AZMR_PlayerCameraPawn::MoveForward(float Value)
{
}

void AZMR_PlayerCameraPawn::MoveRight(float Value)
{
}

void AZMR_PlayerCameraPawn::Zoom(float Value)
{
}
