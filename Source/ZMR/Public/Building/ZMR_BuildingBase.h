// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZMR_BuildingBase.generated.h"

enum class EBuildingType : uint8;
class AZMR_WalkerBase;

UCLASS()
class ZMR_API AZMR_BuildingBase : public AActor
{
  GENERATED_BODY()

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Building")
  TObjectPtr<UStaticMeshComponent> BuildingMesh;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
  int32 Level;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
  int32 Cost;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building|Grid")
  FIntPoint FootprintSize = FIntPoint(1,1);
  
protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
  EBuildingType BuildingType;

private:
  UPROPERTY(VisibleAnywhere, Category="Building")
  int32 BuildingId;

  UPROPERTY(VisibleAnywhere, Category="Building")
  bool bIsPlaced;

  UPROPERTY(VisibleAnywhere, Category="Building|Grid")
  FIntPoint GridPosition = FIntPoint::ZeroValue;

public:
  // Sets default values for this actor's properties
  AZMR_BuildingBase();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  virtual void OnPlaced();

  UFUNCTION(BlueprintCallable)
  virtual void OnDestroyedBuilding();

  UFUNCTION(BlueprintCallable)
  virtual void OnWalkerArrived(const AZMR_WalkerBase* Walker);

  UFUNCTION(BlueprintCallable)
  virtual void UpgradeBuilding();

  UFUNCTION(BlueprintCallable)
  void SetGridPosition(FIntPoint NewGridPosition);

  UFUNCTION(BlueprintPure)
  FIntPoint GetGridPosition() const { return GridPosition; }

  UFUNCTION(BlueprintPure)
  TArray<FIntPoint> GetOccupiedTiles() const;
  
  UFUNCTION(BlueprintPure)
  int32 GetBuildingId() const { return BuildingId; }
};
