// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "ZMR_WalkerBase.generated.h"

enum class EZMR_WalkerStateEnum : uint8;

UCLASS()
class ZMR_API AZMR_WalkerBase : public ACharacter
{
  GENERATED_BODY()

public:
  /* ===== State ===== */
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  EZMR_WalkerStateEnum WalkerState;

  /* ===== Debug ===== */
  UPROPERTY(EditAnywhere)
  bool bDebugPath = true;
  
  UPROPERTY(EditAnywhere)
  float AcceptanceRadius = 75.f;
protected:
private:
  UPROPERTY()
  FVector CurrentTargetLocation;
  UPROPERTY()
  TWeakObjectPtr<AActor> TargetBuilding;

public:
  // Sets default values for this character's properties
  AZMR_WalkerBase();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  
  virtual void PossessedBy(AController* NewController) override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  /* ===== Target Building ===== */

  UFUNCTION(BlueprintCallable)
  void SetTargetBuilding(AActor* InBuilding);

  /* ===== Movement ===== */
  UFUNCTION(BlueprintCallable)
  void MoveToLocation(const FVector& TargetLocation);

  UFUNCTION(BlueprintCallable)
  void StopWalker();


private:
  
  UFUNCTION()
  void OnReachedDestination();
  
  // Bound function to handle when navigation finishes
  UFUNCTION()
  void OnMoveCompleted(struct FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
