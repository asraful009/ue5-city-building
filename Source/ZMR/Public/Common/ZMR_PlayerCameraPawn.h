// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ZMR_PlayerCameraPawn.generated.h"

class UFloatingPawnMovement;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ZMR_API AZMR_PlayerCameraPawn : public APawn
{
  GENERATED_BODY()
  
protected:
  // UPROPERTY(VisibleAnywhere)
  // TWeakObjectPtr<USpringArmComponent> SpringArm;
  //
  // UPROPERTY(VisibleAnywhere)
  // TWeakObjectPtr<UCameraComponent> Camera;
  //
  // UPROPERTY(VisibleAnywhere)
  // TWeakObjectPtr<UFloatingPawnMovement> Movement;
  
public:
  // Sets default values for this pawn's properties
  AZMR_PlayerCameraPawn();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
  
private:
  void MoveForward(float Value);
  void MoveRight(float Value);
  void Zoom(float Value);
  
  
};
