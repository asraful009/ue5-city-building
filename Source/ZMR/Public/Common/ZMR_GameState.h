// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZMR_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDateChanged);

/**
 * 
 */
UCLASS()
class ZMR_API AZMR_GameState : public AGameStateBase
{
  GENERATED_BODY()
  
public:
  UPROPERTY(BlueprintAssignable)
  FOnDateChanged OnDateChanged;
  
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
private:
  
  TArray<FString> Months = { TEXT("UNKNOWN"),
    TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"),
    TEXT("May"), TEXT("Jun"), TEXT("Jul"), TEXT("Aug"),
    TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec")
  };
  
public: 
  AZMR_GameState();
  
  void AdvanceDay();
  
  UFUNCTION(BlueprintCallable)
  FString GetMonthShortName() const;
  
  UFUNCTION(BlueprintCallable)
  FString GetDate() const;
  
protected:
  virtual void BeginPlay() override;
  
  virtual void Tick(float DeltaTime) override;
  
private:
  void InitializeGameState();
  

};
