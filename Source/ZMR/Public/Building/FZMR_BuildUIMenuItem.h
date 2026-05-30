#pragma once

#include "CoreMinimal.h"
#include "Common/FZMR_ResourceStack.h"
#include "Engine/Texture2D.h"
#include "FZMR_BuildUIMenuItem.generated.h"


class AZMR_BuildingBase;

USTRUCT(BlueprintType)
struct FZMR_BuildUIMenuItem
{
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  FText BuildingName;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  UTexture2D* Icon;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  TSubclassOf<AZMR_BuildingBase> BuildingClass;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  int32 BuildingCost = 0;
  
  // CHANGED TO ARRAYS: Allows complex 1:N, N:1, or N:M recipes!
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Logistics")
  TArray<FZMR_ResourceStack> InputResources;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Logistics")
  TArray<FZMR_ResourceStack> OutputResources;
};
