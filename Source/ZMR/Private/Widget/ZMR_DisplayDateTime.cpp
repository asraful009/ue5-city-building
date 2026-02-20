// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ZMR_DisplayDateTime.h"

#include "Common/ZMR_GameState.h"
#include "Components/TextBlock.h"

void UZMR_DisplayDateTime::NativeConstruct()
{
  Super::NativeConstruct();
  GameState = GetWorld() != nullptr
                ? GetWorld()->GetGameState<AZMR_GameState>()
                : nullptr;
  if (GameState.IsValid())
  {
    GameState->OnDateChanged.AddDynamic(this, &UZMR_DisplayDateTime::HandleDateChanged);
  }
}

void UZMR_DisplayDateTime::NativeDestruct()
{
  if (GameState.IsValid())
  {
    GameState->OnDateChanged.RemoveDynamic(this, &UZMR_DisplayDateTime::HandleDateChanged);
    HandleDateChanged(GameState->GetDate());
  }

  Super::NativeDestruct();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UZMR_DisplayDateTime::HandleDateChanged(const FString& NewDate)
{
  if (TxtDate && !NewDate.IsEmpty())
  {
    TxtDate->SetText(FText::FromString(NewDate));
  }
}
