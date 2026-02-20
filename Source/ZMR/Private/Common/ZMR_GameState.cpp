// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ZMR_GameState.h"

AZMR_GameState::AZMR_GameState()
{
  PrimaryActorTick.bCanEverTick = true;
  InitializeGameState();
}

void AZMR_GameState::BeginPlay()
{
  Super::BeginPlay();
}

void AZMR_GameState::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AZMR_GameState::InitializeGameState()
{
  Year = 1897;
  Month = 3;
  Day = 3;
  Gold = 100;
  Population = 10;
}

void AZMR_GameState::AdvanceDay()
{
  Day++;

  if (Day > 30)
  {
    Day = 1;
    Month++;
  }

  if (Month > 12)
  {
    Month = 1;
    Year++;
  }
  UE_LOG(LogTemp, Warning, TEXT("%s"), *GetDate());
  OnDateChanged.Broadcast(GetDate());
}

FString AZMR_GameState::GetMonthShortName() const
{
  if (Month < 1 || Month > 12)
  {
    return TEXT("Invalid Date");
  }
  return Months[Month];
}

FString AZMR_GameState::GetDate() const
{
  return FString::Printf(TEXT("%s %02d, %04d"), *GetMonthShortName(), Day, Year);
}
