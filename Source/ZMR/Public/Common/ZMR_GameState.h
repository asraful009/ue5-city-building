// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZMR_GameState.generated.h"

/**
 * 
 */
UCLASS()
class ZMR_API AZMR_GameState : public AGameStateBase
{
  GENERATED_BODY()
  
protected:
  UPROPERTY(BlueprintReadOnly)
  int32 Day;

  UPROPERTY(BlueprintReadOnly)
  int32 Month;

  UPROPERTY(BlueprintReadOnly)
  int32 Year;

  UPROPERTY(BlueprintReadOnly)
  int32 Gold;

  UPROPERTY(BlueprintReadOnly)
  int32 Population;
  
public: 
  AZMR_GameState();
  
  void AdvanceDay();
  
protected:
  virtual void BeginPlay() override;
  
  virtual void Tick(float DeltaTime) override;
  
private:
  void InitializeGameState();
  

};
