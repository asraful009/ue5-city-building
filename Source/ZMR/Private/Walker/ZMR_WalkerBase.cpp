// Fill out your copyright notice in the Description page of Project Settings.


#include "Walker/ZMR_WalkerBase.h"

#include "Common/Enums/EZMR_GameEnum.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AZMR_WalkerBase::AZMR_WalkerBase()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  WalkerState = EZMR_WalkerStateEnum::Idle;
  TargetBuilding = nullptr;
}

// Called when the game starts or when spawned
void AZMR_WalkerBase::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AZMR_WalkerBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (WalkerState == EZMR_WalkerStateEnum::MovingToTarget)
  {
    UpdateMovement(DeltaTime);
  }
}

// Called to bind functionality to input
void AZMR_WalkerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZMR_WalkerBase::SetTargetBuilding(AActor* InBuilding)
{
  TargetBuilding = InBuilding;

  if (!TargetBuilding.IsValid())
  {
    MoveToLocation(TargetBuilding->GetActorLocation());
  }
}

void AZMR_WalkerBase::MoveToLocation(const FVector& TargetLocation)
{
  CurrentTargetLocation = TargetLocation;
  WalkerState = EZMR_WalkerStateEnum::MovingToTarget;
}

void AZMR_WalkerBase::StopWalker()
{
  WalkerState = EZMR_WalkerStateEnum::Idle;
  GetCharacterMovement()->StopMovementImmediately();
}

void AZMR_WalkerBase::OnReachedDestination()
{
  StopWalker();
  WalkerState = EZMR_WalkerStateEnum::Working;

  UE_LOG(LogTemp, Warning, TEXT("Walker reached destination"));
}

void AZMR_WalkerBase::UpdateMovement(float DeltaTime)
{
  FVector Direction = (CurrentTargetLocation - GetActorLocation()).GetSafeNormal();
  AddMovementInput(Direction);

  float Distance =
    FVector::Dist(GetActorLocation(), CurrentTargetLocation);

  if (bDebugPath)
  {
    DrawDebugLine(GetWorld(),
                  GetActorLocation(),
                  CurrentTargetLocation,
                  FColor::Green,
                  false,
                  -1,
                  0,
                  2.f);
  }
  if (Distance < 100.f)
  {
    OnReachedDestination();
  }

  if (bDebugPath)
  {
    DrawDebugLine(GetWorld(),
                  GetActorLocation(),
                  CurrentTargetLocation,
                  FColor::Green,
                  false,
                  -1,
                  0,
                  2.f);
  }
}
