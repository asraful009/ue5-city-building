// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildingBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AZMR_BuildingBase::AZMR_BuildingBase()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false; // buildings don't need Tick usually

  BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
  RootComponent = BuildingMesh;

  bIsPlaced = false;
  Level = 1;
}

// Called when the game starts or when spawned
void AZMR_BuildingBase::BeginPlay()
{
  Super::BeginPlay();
  TArray<AActor*> Buildings;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZMR_BuildingBase::StaticClass(), Buildings);
  int32 BuildingIdMax = 1;
  for (AActor* Actor : Buildings)
  {
    if (AZMR_BuildingBase* Building = Cast<AZMR_BuildingBase>(Actor))
    {
      if (Building->BuildingId >= BuildingIdMax)
      {
        BuildingIdMax = Building->BuildingId + 1;
      }
    }
  }
  BuildingId = BuildingIdMax;
  UE_LOG(LogTemp, Warning, TEXT("Building ID: %i"), BuildingId);
}

// Called every frame
void AZMR_BuildingBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AZMR_BuildingBase::OnPlaced()
{
  bIsPlaced = true;
  
}

void AZMR_BuildingBase::OnDestroyedBuilding()
{
}

void AZMR_BuildingBase::OnWalkerArrived(const AZMR_WalkerBase* Walker)
{
}

void AZMR_BuildingBase::UpgradeBuilding()
{
}

void AZMR_BuildingBase::SetGridPosition(FIntPoint NewGridPosition)
{
}

TArray<FIntPoint> AZMR_BuildingBase::GetOccupiedTiles() const
{
  TArray<FIntPoint> OccupiedTiles;
  for (int32 X = 0; X < FootprintSize.X; X++)
  {
    for (int32 Y = 0; Y < FootprintSize.Y; Y++)
    {
      OccupiedTiles.Add(FIntPoint(GridPosition.X + X, GridPosition.Y + Y));
    }
  }
  return OccupiedTiles;
}
