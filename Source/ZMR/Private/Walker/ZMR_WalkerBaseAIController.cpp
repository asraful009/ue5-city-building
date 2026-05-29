// Fill out your copyright notice in the Description page of Project Settings.


#include "Walker/ZMR_WalkerBaseAIController.h"

#include "NavigationSystem.h"

void AZMR_WalkerBaseAIController::BeginPlay()
{
  Super::BeginPlay();
  UE_LOG(LogTemp, Warning, TEXT("Walker Base AI Controller BeginPlay"));
}

void AZMR_WalkerBaseAIController::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);
  UE_LOG(LogTemp, Warning, TEXT("AI Controller Possessed Pawn: %s"), *InPawn->GetName());
    
  // Verify NavMesh at possessed pawn location
  const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
  if (NavSys)
  {
    FNavLocation NavLocation;
    if (NavSys->ProjectPointToNavigation(InPawn->GetActorLocation(), NavLocation, FVector(500.0f, 500.0f, 500.0f)))
    {
      UE_LOG(LogTemp, Warning, TEXT("✅ Pawn is ON NavMesh at: %s"), *NavLocation.Location.ToString());
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("❌ Pawn NOT on NavMesh! Spawn location: %s"), *InPawn->GetActorLocation().ToString());
    }
  }
}

void AZMR_WalkerBaseAIController::OnPathFailed(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
  UE_LOG(LogTemp, Error, TEXT("Path finding failed! Result: %d"), (int32)Result);
}
