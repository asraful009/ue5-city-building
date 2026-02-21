// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
  bool bDebugPath = false;

protected:
private:
  FVector CurrentTargetLocation;
  TWeakObjectPtr<AActor> TargetBuilding;

public:
  // Sets default values for this character's properties
  AZMR_WalkerBase();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

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


  UFUNCTION()
  void OnReachedDestination();

private:
  void UpdateMovement(float DeltaTime);
};
