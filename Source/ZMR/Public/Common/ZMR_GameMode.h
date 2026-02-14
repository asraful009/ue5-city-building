// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EZMR_GameEnum.h"
#include "GameFramework/GameModeBase.h"
#include "ZMR_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZMR_API AZMR_GameMode : public AGameModeBase
{
  GENERATED_BODY()

public:
  /* ==============GAME SPEED ================= */
  UPROPERTY(BlueprintReadOnly)
  EZMR_GameSpeedEnum CurrentSpeed;

protected:
private:
  FTimerHandle DayTimerHandle;

public:
  AZMR_GameMode();

  UFUNCTION(BlueprintCallable, Category="ZMR|GameSpeed")
  void SetGameSpeed(const EZMR_GameSpeedEnum NewSpeed);

protected:
  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

public:


private:
  // ------------TIME MANAGEMENT------------
  void HandleDayAdvance() const;

  float GetDurationFromSpeed(const EZMR_GameSpeedEnum Speed) const;
};
