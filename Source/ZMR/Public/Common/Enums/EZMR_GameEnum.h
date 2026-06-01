#pragma once

UENUM(Blueprintable)
enum class EZMR_GameSpeedEnum : uint8
{
  Normal UMETA(DisplayName="Normal"),
  Fast UMETA(DisplayName="Fast"),
  VeryFast UMETA(DisplayName="VeryFast"),
  Pause UMETA(DisplayName="Pause"),
};

UENUM(BlueprintType)
enum class EZMR_WalkerStateEnum : uint8
{ 
  Idle UMETA(DisplayName="Idle"),
  MovingToTarget UMETA(DisplayName="MovingToTarget"),
  Working UMETA(DisplayName="Working"),
  ReturningHome UMETA(DisplayName="ReturningHome")
};

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
  None        UMETA(DisplayName = "None"),
  
  Residential UMETA(DisplayName="Residential"),
  Service     UMETA(DisplayName="Service"),
  Production  UMETA(DisplayName="Production"),
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
  None        UMETA(DisplayName = "None"),
  
  // --- Food Types (For Granaries & Houses) ---
  Rice        UMETA(DisplayName = "Rice"),
  Vegetables  UMETA(DisplayName = "Vegetables"),
  Meat        UMETA(DisplayName = "Meat/Fish"),
  Fruit       UMETA(DisplayName = "Fruit"),
  
  // --- Raw Materials (For Workshops / Warehouses) ---
  Timber      UMETA(DisplayName = "Timber"),
  Clay        UMETA(DisplayName = "Clay"),

  // --- Manufactured Goods (For Distribution / Trade / Luxury Housing) ---
  Pottery     UMETA(DisplayName = "Pottery"),
  Lumber      UMETA(DisplayName = "Lumber"),
  Brick       UMETA(DisplayName = "Brick"),
  Furniture   UMETA(DisplayName = "Furniture"),
  Oil         UMETA(DisplayName = "Oil"),
};
