// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZMR_PlayerController.generated.h"
class AZMR_PlayerCameraPawn;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ZMR_API AZMR_PlayerController : public APlayerController
{
  GENERATED_BODY()
  
protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputMappingContext> IMC_CityBuilder;

  // Input Actions
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_Move;
  
  // Edge threshold in pixels
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  float EdgeThreshold = 100.f;
  
  UPROPERTY(EditAnywhere, Category="Input|Camera")
  float MoveSmoothSpeed = 25.0f;
  
  UPROPERTY(EditAnywhere, Category="Input|Movement")
  float Speed = 2000.0f;
  
private:
  
  // Camera scroll speed
  float EdgeScrollSpeed = 2500.f;

  // State
  bool bIsKeyboardMoving = false;
  float KeyboardMoveTimer = 0.f;  

  
public:
  AZMR_PlayerController();
  
protected:
  
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;
  virtual void Tick(float DeltaTime) override;
private:
  void FocusCameraOnSelected();

  // Camera edge scroll
  void HandleEdgeScroll(float DeltaTime);
  
  void Move(const FInputActionValue& Value);
  
};
