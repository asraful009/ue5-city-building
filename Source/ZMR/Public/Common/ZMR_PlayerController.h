// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZMR_PlayerController.generated.h"
class AZMR_BuildingBase;
class UZMR_BuildMenuWidget;
class AZMR_PlayerCameraPawn;
class AZMR_WalkerBase;
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
  
  // Input Actions
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_Zoom;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_Rotate;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_MouseHold;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_MouseDrag;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  TObjectPtr<UInputAction> IA_MouseSelect;
  
  // Edge threshold in pixels
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
  float EdgeThreshold = 10.f;
  
  UPROPERTY(EditAnywhere, Category="Input|Camera")
  float MoveSmoothSpeed = 25.0f;
  
  UPROPERTY(EditAnywhere, Category="Input|Movement")
  float Speed = 2000.0f;
  
  UPROPERTY(EditAnywhere, Category="Input|Camera")
  float DragSpeed = 1.f;
  
  UPROPERTY(EditAnywhere, Category="UI|Building")
  TSubclassOf<UZMR_BuildMenuWidget> BuildMenuClass;
  
private:
  
  // Camera scroll speed
  float EdgeScrollSpeed = 2500.f;

  // State
  bool bIsKeyboardMoving = false;
  float KeyboardMoveTimer = 0.f;  
  
  bool bIsMouseHolding = false;

  
  // For smoothing
  FVector2D DragStartMousePos = FVector2D::ZeroVector;
  FVector DragStartCameraPawnPos = FVector::ZeroVector;
  
  UPROPERTY()
  TWeakObjectPtr<AZMR_PlayerCameraPawn> CameraPawnRef;
  
  UPROPERTY()
  TWeakObjectPtr<AActor> MouseSelectedActorRef;
  
  UPROPERTY()
  TWeakObjectPtr<AZMR_WalkerBase> LocalWalkerActorRef;
  
  UPROPERTY()
  TWeakObjectPtr<AZMR_BuildingBase> CurrentPreviewBuilding;
  
public:
  AZMR_PlayerController();
  
protected:
  
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;
  virtual void Tick(float DeltaTime) override;

public:
  UFUNCTION()
  void StartPlacingBuilding(const TSubclassOf<AZMR_BuildingBase> NewPreviewBuildingClass);

private:
  UFUNCTION()
  void FocusCameraOnSelected();

  // Camera edge scroll
  UFUNCTION()
  void HandleEdgeScroll(float DeltaTime);
  
  UFUNCTION()
  void Move(const FInputActionValue& Value);
  
  UFUNCTION()
  void Zoom(const FInputActionValue& Value);
  
  UFUNCTION()
  void RotateCamera(const FInputActionValue& Value);
  
  UFUNCTION()
  void MouseDragStart(const FInputActionValue& Value);
  
  UFUNCTION()
  void MouseDragStop(const FInputActionValue& Value);
  
  UFUNCTION()
  void MouseDragMove(const FInputActionValue& Value);
  
  UFUNCTION()
  void MouseSelectObject();
  
  
  void MouseSelectedObject(const TObjectPtr<AActor> NewSelectedObject);
  
  UFUNCTION()
  void MouseDeSelectedObject();
  
  UFUNCTION()
  void MoveBuildingWithMouse(const float DeltaTime);
  
};
