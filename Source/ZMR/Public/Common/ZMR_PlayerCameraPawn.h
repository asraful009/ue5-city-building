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
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
  TObjectPtr<USpringArmComponent> SpringArm;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
  TObjectPtr<UCameraComponent> Camera;
  
  UPROPERTY(EditAnywhere, Category="Camera")
  float ZoomSpeed = 200.f;

  UPROPERTY(EditAnywhere, Category="Camera")
  float MinZoom = 500.f;

  UPROPERTY(EditAnywhere, Category="Camera")
  float MaxZoom = 2000.f;
  
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
  
  void HandleZoom(float Value);

private:
  void MoveForward(float Value);
  void MoveRight(float Value);
  
  
};
