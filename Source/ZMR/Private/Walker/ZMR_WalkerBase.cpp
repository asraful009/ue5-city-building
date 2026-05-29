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
  GetCharacterMovement()->MaxWalkSpeed = 300.f;
  GetCharacterMovement()->bOrientRotationToMovement = true;
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
  // Debug: draw line from pawn to target
  if (bDebugPath && WalkerState == EZMR_WalkerStateEnum::MovingToTarget)
  {
    DrawDebugLine(GetWorld(),
                  GetActorLocation(),
                  CurrentTargetLocation,
                  FColor::Green, false, -1, 0, 2.f);
    
    DrawDebugCircle(
          GetWorld(),
          GetActorLocation(),
          AcceptanceRadius,
          32,
          FColor::Green,
          false,          // NOT persistent (important)
          0.f,            // lifetime = 0 means only this frame
          0,
          2.f,
          FVector(1,0,0),
          FVector(0,1,0),
          false
      );
    DrawDebugCircle(
        GetWorld(),
        CurrentTargetLocation,
        AcceptanceRadius,
        32,
        FColor::Red,
        false,          // NOT persistent (important)
        0.f,            // lifetime = 0 means only this frame
        0,
        2.f,
        FVector(1,0,0),
        FVector(0,1,0),
        false
    );
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
    return;
  }
  FVector Origin, BoxExtent;
  InBuilding->GetActorBounds(false, Origin, BoxExtent);
  FVector GroundTarget = Origin;
  GroundTarget.Z = Origin.Z - BoxExtent.Z;  // bottom of bounds
  MoveToLocation(GroundTarget);
}

void AZMR_WalkerBase::MoveToLocation(const FVector& TargetLocation)
{
  
  CurrentTargetLocation = TargetLocation;
  UE_LOG(LogTemp, Warning, TEXT("Walker is Set Target to location: %s"), *TargetLocation.ToString());
  
  const TObjectPtr<AZMR_WalkerBaseAIController> WalkerController = Cast<AZMR_WalkerBaseAIController>(GetController());
  if (!WalkerController)
  {
    UE_LOG(LogTemp, Warning, TEXT("WalkerController is null"));
    WalkerState = EZMR_WalkerStateEnum::Idle;
    return;
  }
  WalkerController->StopMovement();
  // First, project the target location to NavMesh to verify it exists
  const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
  if (NavSys)
  {
    UE_LOG(LogTemp, Warning, TEXT(">>> Walker is checking NavMesh for location: %s"), *TargetLocation.ToString());
    FNavLocation NavLocation;
    if (NavSys->ProjectPointToNavigation(CurrentTargetLocation, NavLocation, FVector(1000.0f, 1000.0f, 1000.0f)))
    {
      // Use the NavMesh-corrected location
      CurrentTargetLocation = NavLocation.Location;
      UE_LOG(LogTemp, Warning, TEXT(">>> Target projected to NavMesh: %s"), *CurrentTargetLocation.ToString());
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT(">>> Target location is NOT on NavMesh! X=%.0f Y=%.0f Z=%.0f"), 
          CurrentTargetLocation.X, CurrentTargetLocation.Y, CurrentTargetLocation.Z);
            
      // Try to find any valid NavMesh point near the target
      if (NavSys->GetRandomReachablePointInRadius(CurrentTargetLocation, 1000.0f, NavLocation))
      {
        CurrentTargetLocation = NavLocation.Location;
        UE_LOG(LogTemp, Warning, TEXT(">>> Using nearby NavMesh point: %s"), *CurrentTargetLocation.ToString());
      }
      else
      {
        UE_LOG(LogTemp, Error, TEXT(">>> No NavMesh found within 1000 units of target!"));
        WalkerState = EZMR_WalkerStateEnum::Idle;
        return;
      }
    }
  }
  
  // Use the simplest version first
  const EPathFollowingRequestResult::Type Result = WalkerController->MoveToLocation(
      CurrentTargetLocation,
      AcceptanceRadius,
      true,  // bStopOnOverlap
      true,  // bUsePathfinding
      true,  // bProjectDestinationToNavigation - this is key!
      true   // bCanStrafe
  );
  UE_LOG(LogTemp, Warning, TEXT("Move Result: %d : %s"), (int32) Result, *CurrentTargetLocation.ToString()); 
  if (Result == EPathFollowingRequestResult::RequestSuccessful)
  {
    UE_LOG(LogTemp, Warning, TEXT("Move Request Successful"));
    WalkerState = EZMR_WalkerStateEnum::MovingToTarget;
  } 
  else if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
  {
    UE_LOG(LogTemp, Warning, TEXT("Already at goal location!"));
    OnReachedDestination();
  }
  else if (Result == EPathFollowingRequestResult::Failed)
  {
    UE_LOG(LogTemp, Error, TEXT(">>>> ❌ MoveTo Failed! Check if NavMesh exists."));
    WalkerState = EZMR_WalkerStateEnum::Idle;
  } else
  {
    UE_LOG(LogTemp, Error, TEXT("❌ MoveTo UNKNOWN ! Check if NavMesh exists."));
    WalkerState = EZMR_WalkerStateEnum::Idle;
  }
 
}

void AZMR_WalkerBase::StopWalker()
{
  WalkerState = EZMR_WalkerStateEnum::Idle;
  GetCharacterMovement()->StopMovementImmediately();
}

void AZMR_WalkerBase::DebugNavMeshAtLocation(const FVector& Location)
{
}

bool AZMR_WalkerBase::IsLocationOnNavMesh(const FVector& Location)
{
  UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
  if (!NavSystem) return false;
    
  FNavLocation NavLocation;
  return NavSystem->ProjectPointToNavigation(Location, NavLocation, FVector(500.0f, 500.0f, 500.0f));
}

void AZMR_WalkerBase::OnReachedDestination()
{  

  StopWalker();
  UE_LOG(LogTemp, Warning, TEXT("Walker reached destination : %s"), *CurrentTargetLocation.ToString());
  
}

void AZMR_WalkerBase::OnMoveCompleted(struct FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
  if (Result == EPathFollowingResult::Success)
  {
    UE_LOG(LogTemp, Warning, TEXT("Walker reached destination"));
    OnReachedDestination();
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Walker failed to reach destination"));
  } 

}

void AZMR_WalkerBase::CheckNavMeshAvailability()
{
  
}
