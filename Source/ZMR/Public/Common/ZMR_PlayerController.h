// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZMR_PlayerController.generated.h"
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
  
private:
  
  // Camera scroll speed
  float EdgeScrollSpeed = 2500.f;

  // Edge threshold in pixels
  float EdgeThreshold = 20.f;
  
  
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
