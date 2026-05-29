// Fill out your copyright notice in the Description page of Project Settings.


#include "Walker/ZMR_WalkerBase.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Common/Enums/EZMR_GameEnum.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Walker/ZMR_WalkerBaseAIController.h"

// Sets default values
AZMR_WalkerBase::AZMR_WalkerBase()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  WalkerState = EZMR_WalkerStateEnum::Idle;
  TargetBuilding = nullptr;
  AIControllerClass = AZMR_WalkerBaseAIController::StaticClass();
  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AZMR_WalkerBase::BeginPlay()
{
  Super::BeginPlay();
  const TObjectPtr<AZMR_WalkerBaseAIController> WalkerController = Cast<AZMR_WalkerBaseAIController>(GetController());
}

void AZMR_WalkerBase::PossessedBy(AController* NewController)
{
  Super::PossessedBy(NewController);
  if (!NewController)
  {
    UE_LOG(LogTemp, Warning, TEXT("NewController is null"));
    return;
  }
  const TObjectPtr<AZMR_WalkerBaseAIController> WalkerController = Cast<AZMR_WalkerBaseAIController>(NewController);
  if (!WalkerController)
  {
    UE_LOG(LogTemp, Warning, TEXT("WalkerController is null"));
    return;
  }
  WalkerController->ReceiveMoveCompleted.AddDynamic(this, &AZMR_WalkerBase::OnMoveCompleted);
  UE_LOG(LogTemp, Warning, TEXT("Walker AI Possessed"));
}

// Called every frame
void AZMR_WalkerBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZMR_WalkerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZMR_WalkerBase::SetTargetBuilding(AActor* InBuilding)
{
  TargetBuilding = InBuilding;

  if (TargetBuilding.IsValid())
  {
    MoveToLocation(TargetBuilding->GetActorLocation());
  }
}

void AZMR_WalkerBase::MoveToLocation(const FVector& TargetLocation)
{
  CurrentTargetLocation = TargetLocation;
  WalkerState = EZMR_WalkerStateEnum::MovingToTarget;
  UE_LOG(LogTemp, Warning, TEXT("Walker is Set Target to location: %s"), *TargetLocation.ToString());
  const TObjectPtr<AZMR_WalkerBaseAIController> WalkerController = Cast<AZMR_WalkerBaseAIController>(GetController());
  if (!WalkerController)
  {
    UE_LOG(LogTemp, Warning, TEXT("WalkerController is null"));
    WalkerState = EZMR_WalkerStateEnum::Idle;
    return;
  }
  FAIMoveRequest MoveRequest;
  MoveRequest.SetGoalLocation(CurrentTargetLocation);
  MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
  MoveRequest.SetUsePathfinding(true);
  FNavPathSharedPtr NavPath;
  
  const EPathFollowingRequestResult::Type Result =
    WalkerController->MoveTo(MoveRequest, &NavPath);

  UE_LOG(LogTemp, Warning, TEXT("Move Result: %d : %s"), (int32)Result, *CurrentTargetLocation.ToString());  
}

void AZMR_WalkerBase::StopWalker()
{
  WalkerState = EZMR_WalkerStateEnum::Idle;
  GetCharacterMovement()->StopMovementImmediately();
}

void AZMR_WalkerBase::OnReachedDestination()
{  

  StopWalker();
  WalkerState = EZMR_WalkerStateEnum::Idle;
  UE_LOG(LogTemp, Warning, TEXT("Walker reached destination : %s"), *CurrentTargetLocation.ToString());
  
}

void AZMR_WalkerBase::OnMoveCompleted(struct FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
  if (Result == EPathFollowingResult::Success)
  {
    UE_LOG(LogTemp, Warning, TEXT("Walker reached destination"));
    
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Walker failed to reach destination"));
  } 
  OnReachedDestination();
}
