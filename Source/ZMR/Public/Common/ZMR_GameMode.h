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
  /* =========================
     TIMER
  ========================== */

  FTimerHandle DayTimerHandle;

  void HandleDayAdvance();

  float GetDurationFromSpeed(EZMR_GameSpeedEnum Speed) const;

public:
  AZMR_GameMode();

protected:
  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

public:
  UFUNCTION(BlueprintCallable)
  void SetGameSpeed(const EZMR_GameSpeedEnum NewSpeed);

private:
  // ------------TIME MANAGEMENT------------
  void HandleDayAdvance() const;

  float GetDurationFromSpeed(const EZMR_GameSpeedEnum Speed) const;
};
