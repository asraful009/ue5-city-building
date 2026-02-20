// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZMR_DisplayDateTime.generated.h"

class AZMR_GameState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ZMR_API UZMR_DisplayDateTime : public UUserWidget
{
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> TxtDate;

  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;
  
  UFUNCTION()
  void HandleDateChanged(const FString& NewDate);
  
private:
  UPROPERTY()
  TWeakObjectPtr<AZMR_GameState> GameState;
};
