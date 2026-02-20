#pragma once

UENUM(Blueprintable)
enum class EZMR_GameSpeedEnum : uint8
{
  Pause UMETA(DisplayName="Pause"),
  Normal UMETA(DisplayName="Normal"),
  Fast UMETA(DisplayName="Fast"),
  VeryFast UMETA(DisplayName="VeryFast")
};

UENUM(BlueprintType)
enum class EZMR_WalkerStateEnum : uint8
{
  Idle UMETA(DisplayName="Idle"),
  MovingToTarget UMETA(DisplayName="MovingToTarget"),
  Working UMETA(DisplayName="Working"),
  ReturningHome UMETA(DisplayName="ReturningHome")
};