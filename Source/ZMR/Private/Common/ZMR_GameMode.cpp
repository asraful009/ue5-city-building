// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_GameMode.h"

#include "Common/ZMR_GameState.h"

AZMR_GameMode::AZMR_GameMode()
{
  PrimaryActorTick.bCanEverTick = true;
  CurrentSpeed = EZMR_GameSpeedEnum::Normal;
}

void AZMR_GameMode::BeginPlay()
{
  Super::BeginPlay();

  SetGameSpeed(EZMR_GameSpeedEnum::Normal);
  UE_LOG(LogTemp, Warning, TEXT("Game Started - Timer Initialized"));
}

void AZMR_GameMode::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AZMR_GameMode::SetGameSpeed(const EZMR_GameSpeedEnum NewSpeed)
{
  CurrentSpeed = NewSpeed;

  GetWorldTimerManager().ClearTimer(DayTimerHandle);
  const float Duration = GetDurationFromSpeed(NewSpeed);
  if (Duration < 0.f)
  {
    UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
    return;
  }

  GetWorldTimerManager().SetTimer(
    DayTimerHandle,
    this,
    &AZMR_GameMode::HandleDayAdvance,
    Duration,
    true
  );

  UE_LOG(LogTemp, Warning, TEXT("Game Speed Changed"));
}

void AZMR_GameMode::HandleDayAdvance() const
{
  const TObjectPtr<AZMR_GameState> GS =
    GetGameState<AZMR_GameState>();
  if (GS != nullptr)
  {
    GS->AdvanceDay();
  }
}

float AZMR_GameMode::GetDurationFromSpeed(const EZMR_GameSpeedEnum Speed) const
{
  switch (Speed)
  {
  case EZMR_GameSpeedEnum::Normal:
    return 2.f;
  case EZMR_GameSpeedEnum::Fast:
    return 1.0f;
  case EZMR_GameSpeedEnum::VeryFast:
    return 0.25f;
  case EZMR_GameSpeedEnum::Pause:
  default:
    return -1.f;
  }
}
