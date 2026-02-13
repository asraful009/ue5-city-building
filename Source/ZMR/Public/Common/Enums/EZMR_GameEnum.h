#pragma once

UENUM(Blueprintable)
enum class EZMR_GameSpeedEnum : uint8
{
  Pause UMETA(DisplayName="Pause"),
  Normal UMETA(DisplayName="Normal"),
  Fast UMETA(DisplayName="Fast"),
  VeryFast UMETA(DisplayName="VeryFast")
};
