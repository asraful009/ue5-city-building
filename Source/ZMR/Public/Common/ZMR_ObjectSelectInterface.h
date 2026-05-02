// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZMR_ObjectSelectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UZMR_ObjectSelectInterface : public UInterface
{
  GENERATED_BODY()
};

/**
 * 
 */
class ZMR_API IZMR_ObjectSelectInterface
{
  GENERATED_BODY()

  // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Selection")
  void OnSelected();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Selection")
  void OnDeselected();
};
