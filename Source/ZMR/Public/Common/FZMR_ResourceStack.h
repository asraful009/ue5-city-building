#pragma once

#include "CoreMinimal.h"
#include "Enums/EZMR_GameEnum.h"
#include "FZMR_ResourceStack.generated.h"

USTRUCT(BlueprintType)
struct FZMR_ResourceStack
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
  EResourceType ResourceType = EResourceType::None;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
  int32 Quantity = 0;
  
  // optional but VERY useful later
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float AmountPerCycle = 0.f;
};
