
# UE5 Building Menu Item Structs
`FZMR_ResourceStack` and `FZMR_BuildUIMenuItem` are the two main structs used for managing building menu items in the game.
create a data table for it. 

```CPP

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

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Common/Enums/EZMR_GameEnum.h"
#include "Common/FZMR_ResourceStack.h"
#include "FZMR_BuildUIMenuItem.generated.h"


class AZMR_BuildingBase;
class UTexture2D;

USTRUCT(BlueprintType)
struct FZMR_BuildUIMenuItem : public FTableRowBase
{
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Build Menu Item")
  EBuildingType BuildingType = EBuildingType::None;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  FText BuildingName;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Build Menu Item")
  FText BuildingDescription = FText::FromString("No description available.");
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  TObjectPtr<UTexture2D> Icon;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  TSubclassOf<AZMR_BuildingBase> BuildingClass;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Build Menu Item")
  int32 BuildingCost = 0;
      
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Build Menu Item")
  float BuildTime = 0.f;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Build Menu Item")
  int32 WorkerRequired = 0;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Build Menu Item")
  int32 SortPriority = 0;
  
  // CHANGED TO ARRAYS: Allows complex 1:N, N:1, or N:M recipes!
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Logistics")
  TArray<FZMR_ResourceStack> InputResources;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Logistics")
  TArray<FZMR_ResourceStack> OutputResources;

};

```